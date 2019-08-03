extends Node2D

onready var lesspass_data = preload("res://bin/lesspass.gdns").new()

func _ready():
	var result = lesspass_data.get_data("test", "testing", "password")
	if typeof(result) == TYPE_INT:
		printerr("GDNATIVE BINDINGS ERROR")
	elif typeof(result) == TYPE_STRING:
		print(result)
	else:
		printerr("UNKNOWN ERROR")