extends AnimationPlayer

func _ready():
	# adjust animation to screen size
	var monitor_resolution: Vector2 = OS.get_screen_size(OS.get_current_screen())
	var enter_animation = get_animation("enter")
	enter_animation.track_set_key_value(1, 0, Vector2(0, -monitor_resolution.y))
	var exit_animation = get_animation("exit")
	exit_animation.track_set_key_value(2, 1, Vector2(0, monitor_resolution.y))
	
	play("enter")