package main

import (
	"fmt"
	"os"

	MQTT "github.com/eclipse/paho.mqtt.golang"
	"github.com/mymmrac/telego"
	th "github.com/mymmrac/telego/telegohandler"
	tu "github.com/mymmrac/telego/telegoutil"
)

var broker, username, password = "tcp://m7.wqtt.ru:15128", "u_QPTT9R", "fAhauOpC"

func mqtt32() {

	// Создание клиента
	opts := MQTT.NewClientOptions().AddBroker(broker)
	opts.SetUsername(username)
	opts.SetPassword(password)
	opts.SetClientID("go_mqtt_client")

	// Создание клиента
	client := MQTT.NewClient(opts)

	// Подключение к брокеру
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		fmt.Println(token.Error())
		os.Exit(1)
	}
	defer client.Disconnect(250)

	// публикация сообщения
	token := client.Publish("new/button", 0, false, "45")
	token.Wait()
	fmt.Println("Message #32 published")
}

func mqtt33() {

	// Создание клиента
	opts := MQTT.NewClientOptions().AddBroker(broker)
	opts.SetUsername(username)
	opts.SetPassword(password)
	opts.SetClientID("go_mqtt_client")

	// Создание клиента
	client := MQTT.NewClient(opts)

	// Подключение к брокеру
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		fmt.Println(token.Error())
		os.Exit(1)
	}
	defer client.Disconnect(250)

	// публикация сообщения
	token := client.Publish("new/button", 0, false, "46")
	token.Wait()
	fmt.Println("Message #33 published")
}

func main() {
	botToken := "7827011556:AAEW2JiNoBi86IbqPK656Pi9_4KAwjk3pFI"         //токен для тг-бота
	bot, err := telego.NewBot(botToken, telego.WithDefaultDebugLogger()) //инициализация бота
	//проверка на ошибки
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	//клавиатура для tg-бота
	keyboard := tu.Keyboard(
		tu.KeyboardRow(

			tu.KeyboardButton("/open_lock32"),
			tu.KeyboardButton("/open_lock33"),
		),
		tu.KeyboardRow(

			tu.KeyboardButton("/help"),
		),
	)
	updates, _ := bot.UpdatesViaLongPolling(nil)
	bh, _ := th.NewBotHandler(bot, updates)
	defer bh.Stop()             //остановка Хендлов(для кондийций)
	defer bot.StopLongPolling() //стек

	//обработка команды /start
	bh.Handle(func(bot *telego.Bot, update telego.Update) {

		chatID := tu.ID(update.Message.Chat.ID)
		message := tu.Message(
			chatID,
			"MQTT-панель для Системы контроля и управления доступом.Откройте доступные Вам кабинеты").WithReplyMarkup(keyboard)
		_, _ = bot.SendMessage(message)

	}, th.CommandEqual("start"))

	//обработчик команды /help
	bh.Handle(func(bot *telego.Bot, update telego.Update) {
		chatID := tu.ID(update.Message.Chat.ID)
		message := tu.Message(
			chatID,
			" Cоздано на языке Go с использованием библиотек 'TeleGO' и 'Paho-MQTT', по всем вопросам ").WithReplyMarkup(keyboard)
		_, _ = bot.SendMessage(message)

	}, th.CommandEqual("help"))
	//обработчик комманды для открытия двери
	bh.Handle(func(bot *telego.Bot, update telego.Update) {
		chatID := tu.ID(update.Message.Chat.ID)
		message := tu.Message(
			chatID,
			"Кабинет №32 открыт!",
		).WithReplyMarkup(keyboard)
		_, _ = bot.SendMessage(message)
		mqtt32()
	}, th.CommandEqual("open_lock32"))
	bh.Handle(func(bot *telego.Bot, update telego.Update) {
		chatID := tu.ID(update.Message.Chat.ID)
		message := tu.Message(
			chatID,
			"Кабинет №33 открыт!",
		)
		_, _ = bot.SendMessage(message)
		mqtt33()
	}, th.CommandEqual("open_lock33"))
	bh.Handle(func(bot *telego.Bot, update telego.Update) {

	}, th.CommandEqual("иван_богданов"))
	bh.Start()
}
