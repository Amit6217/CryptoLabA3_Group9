from datetime import datetime
import os

LOG_FILE = "logs/activity.log"

def log_activity(option):
    """Logs the selected menu option with a timestamp."""
    # Ensure the logs directory exists
    os.makedirs(os.path.dirname(LOG_FILE), exist_ok=True)
    with open(LOG_FILE, "a") as file:
        current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        file.write(f"{current_time} - {option}\n")
