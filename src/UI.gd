extends CanvasLayer

onready var lesspass_data = preload("res://bin/lesspass.gdns").new()

func _on_GenerateButton_pressed():
	var main_vbox_container = $PanelContainer/MarginContainer/MainVBoxContainer
	var site_name = main_vbox_container.get_node("SiteName").text
	var user_name = main_vbox_container.get_node("UserName").text
	var password = main_vbox_container.get_node("MasterPassword").text
	var result = lesspass_data.get_data(site_name, user_name, password)
	password = null
	main_vbox_container.get_node("MasterPassword").text = ""
	if typeof(result) == TYPE_INT:
		printerr("GDNATIVE BINDINGS ERROR")
	elif typeof(result) == TYPE_STRING:
		var password_label = main_vbox_container.get_node("PasswordPanel/PasswordHBox/PasswordLabel")
		password_label.visible = true
		print(result)
		password_label.text = result
	else:
		printerr("UNKNOWN ERROR")
