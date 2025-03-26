package main

import (
	"fmt"
	"os"
	"sync"

	MQTT "github.com/eclipse/paho.mqtt.golang"
	"github.com/mymmrac/telego"
	th "github.com/mymmrac/telego/telegohandler"
	tu "github.com/mymmrac/telego/telegoutil"
)

// Конфигурационные параметры
type Config struct {
	MQTTBroker   string
	MQTTUsername string
	MQTTPassword string
	BotToken     string
}

// Room представляет информацию о кабинете
type Room struct {
	ID      int
	Name    string
	Command string
	Topic   string
	Message string
}

var (
	config = Config{
		MQTTBroker:   "tcp://m7.wqtt.ru:15128",
		MQTTUsername: "u_QPTT9R",
		MQTTPassword: "fAhauOpC",
		BotToken:     "7827011556:AAEW2JiNoBi86IbqPK656Pi9_4KAwjk3pFI",
	}

	// Список доступных кабинетов
	rooms = []Room{
		{
			ID:      32,
			Name:    "Кабинет №32",
			Command: "open_lock32",
			Topic:   "new/button",
			Message: "32",
		},
		{
			ID:      33,
			Name:    "Кабинет №33",
			Command: "open_lock33",
			Topic:   "new/button",
			Message: "33",
		},
	}

	mqttClient MQTT.Client
	clientOnce sync.Once
)

// getMQTTClient возвращает singleton клиента MQTT
func getMQTTClient() (MQTT.Client, error) {
	var err error
	clientOnce.Do(func() {
		opts := MQTT.NewClientOptions().AddBroker(config.MQTTBroker)
		opts.SetUsername(config.MQTTUsername)
		opts.SetPassword(config.MQTTPassword)
		opts.SetClientID("go_mqtt_client")

		client := MQTT.NewClient(opts)
		if token := client.Connect(); token.Wait() && token.Error() != nil {
			err = token.Error()
			return
		}
		mqttClient = client
	})

	return mqttClient, err
}

// publishMQTTMessage публикует сообщение в MQTT
func publishMQTTMessage(topic, message string) error {
	client, err := getMQTTClient()
	if err != nil {
		return fmt.Errorf("MQTT connection failed: %w", err)
	}

	token := client.Publish(topic, 0, false, message)
	token.Wait()
	if token.Error() != nil {
		return fmt.Errorf("MQTT publish failed: %w", token.Error())
	}

	return nil
}

func createKeyboard() *telego.ReplyKeyboardMarkup {
	// Количество кнопок в строке
	buttonsPerRow := 2

	// Создаем слайс для всех строк
	rows := make([][]telego.KeyboardButton, 0)

	// Создаем временную строку
	currentRow := make([]telego.KeyboardButton, 0, buttonsPerRow)

	// Добавляем кнопки для кабинетов
	for _, room := range rooms {
		btn := tu.KeyboardButton("/" + room.Command)
		currentRow = append(currentRow, btn)

		// Если строка заполнена, добавляем в rows и создаем новую
		if len(currentRow) >= buttonsPerRow {
			rows = append(rows, currentRow)
			currentRow = make([]telego.KeyboardButton, 0, buttonsPerRow)
		}
	}

	// Добавляем оставшиеся кнопки, если они есть
	if len(currentRow) > 0 {
		rows = append(rows, currentRow)
	}

	// Добавляем кнопку помощи в отдельную строку
	rows = append(rows, tu.KeyboardRow(
		tu.KeyboardButton("/help"),
	))

	return tu.Keyboard(rows...).
		WithResizeKeyboard(). // Автоматическое изменение размера
		WithOneTimeKeyboard() // Скрытие клавиатуры после использования
}

func main() {
	// Инициализация бота
	bot, err := telego.NewBot(config.BotToken, telego.WithDefaultDebugLogger())
	if err != nil {
		fmt.Printf("Failed to create bot: %v\n", err)
		os.Exit(1)
	}

	// Получение обновлений
	updates, err := bot.UpdatesViaLongPolling(nil)
	if err != nil {
		fmt.Printf("Failed to get updates: %v\n", err)
		os.Exit(1)
	}

	// Создание обработчика
	bh, err := th.NewBotHandler(bot, updates)
	if err != nil {
		fmt.Printf("Failed to create bot handler: %v\n", err)
		os.Exit(1)
	}

	defer func() {
		bh.Stop()
		bot.StopLongPolling()
		if mqttClient != nil && mqttClient.IsConnected() {
			mqttClient.Disconnect(250)
		}
	}()

	keyboard := createKeyboard()

	// Обработчик команды /start
	bh.Handle(func(bot *telego.Bot, update telego.Update) {
		chatID := tu.ID(update.Message.Chat.ID)
		message := tu.Message(
			chatID,
			"MQTT-панель для Системы контроля и управления доступом. Откройте доступные Вам кабинеты",
		).WithReplyMarkup(keyboard)
		_, _ = bot.SendMessage(message)
	}, th.CommandEqual("start"))

	// Обработчик команды /help
	bh.Handle(func(bot *telego.Bot, update telego.Update) {
		chatID := tu.ID(update.Message.Chat.ID)
		message := tu.Message(
			chatID,
			"Этот бот создан для управления системой контроля доступа.\n\n"+
				"Используйте кнопки ниже для открытия соответствующих кабинетов.\n\n"+
				"Создано на языке Go с использованием библиотек 'TeleGO' и 'Paho-MQTT'.",
		).WithReplyMarkup(keyboard)
		_, _ = bot.SendMessage(message)
	}, th.CommandEqual("help"))

	// Обработчики для каждого кабинета
	for _, room := range rooms {
		room := room // создаем локальную копию для замыкания
		bh.Handle(func(bot *telego.Bot, update telego.Update) {
			chatID := tu.ID(update.Message.Chat.ID)

			// Отправляем сообщение об открытии
			_, _ = bot.SendMessage(tu.Message(
				chatID,
				fmt.Sprintf("%s открыт!", room.Name),
			).WithReplyMarkup(keyboard))

			// Публикуем MQTT сообщение
			if err := publishMQTTMessage(room.Topic, room.Message); err != nil {
				_, _ = bot.SendMessage(tu.Message(
					chatID,
					fmt.Sprintf("Ошибка при открытии %s: %v", room.Name, err),
				))
				return
			}

			fmt.Printf("%s opened\n", room.Name)
		}, th.CommandEqual(room.Command))
	}

	// Запуск обработчика
	bh.Start()
}
