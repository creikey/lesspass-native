extends CanvasLayer

onready var lesspass_data = preload("res://bin/lesspass.gdns").new()

func _ready():
	get_tree().get_root().set_transparent_background(true)
	OS.window_size = OS.get_screen_size(OS.get_current_screen())
	OS.window_position = Vector2()

func _process(delta):
	OS.window_position = Vector2()

func _on_GenerateButton_pressed():
	var main_vbox_container = $MarginContainer/PanelContainer/MarginContainer/MainVBoxContainer
	var generate_button = main_vbox_container.get_node("PasswordPanel/PasswordHBox/GenerateButton")
	var password_label = main_vbox_container.get_node("PasswordPanel/PasswordHBox/PasswordLabel")
	if generate_button.text == "":
		OS.clipboard = password_label.text
		return
	var site_name = main_vbox_container.get_node("SiteName").text
	var user_name = main_vbox_container.get_node("UserName").text
	var password = main_vbox_container.get_node("MasterPassword").text
	var result = lesspass_data.get_data(site_name, user_name, password)
	password = null
	main_vbox_container.get_node("MasterPassword").text = ""
	if typeof(result) == TYPE_INT:
		printerr("GDNATIVE BINDINGS ERROR")
	elif typeof(result) == TYPE_STRING:

		password_label.visible = true
		print(result)
		password_label.text = result

		generate_button.text = ""
		generate_button.icon = preload("res://copy-to-clipboard.svg")
	else:
		printerr("UNKNOWN ERROR")

func exit():
	get_tree().quit()