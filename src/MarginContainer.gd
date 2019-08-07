extends MarginContainer

func _gui_input(event):
	if event.is_action_pressed("ui_exit"):
		get_node("/root/Main/AnimationPlayer").play("exit")