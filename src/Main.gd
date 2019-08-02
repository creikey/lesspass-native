extends Node2D

onready var lesspass_data = preload("res://bin/lesspass.gdns").new()

func _ready():
	print(lesspass_data.get_data())