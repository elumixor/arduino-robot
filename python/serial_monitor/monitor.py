from serial import Serial
from .message_reader import MessageReader
from .serial_message_reader import SerialMessageReader

class SerialMonitor:
    def __init__(self, port="/dev/ttyACM0", baudrate=9600):
        self.__serial = Serial(port, baudrate)
        self.__serial_message_reader = SerialMessageReader(self.__serial)
        self.__message_reader = MessageReader()
       
    def __enter__(self):
        self.__serial.__enter__()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.__serial.__exit__(exc_type, exc_value, traceback)

    def run(self):
        while True:
            serial_message = self.__serial_message_reader.check_message()
            if serial_message is not None:
                print(f"\33[2K\r>> {serial_message}")

            message = self.__message_reader.check_message()
            if message is not None:
                message_length = len(message)
                message_length_b = message_length.to_bytes(2, byteorder="little")

                self.__serial.write(message_length_b)
                self.__serial.write(message.encode())


if __name__ == "__main__":
    with SerialMonitor() as monitor:
        monitor.run()
