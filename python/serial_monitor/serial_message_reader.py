class SerialMessageReader:
    def __init__(self, serial):
        self.__serial = serial
        self.__current_message = None
        self.__current_expected_length = None
        self.__current_expected_length_bytes = b''

    def check_message(self):
        while self.__serial.in_waiting > 0:
            if self.__current_expected_length is None:
                self.__receive_length()
                continue

            self.__current_message += self.__serial.read(1)
            if len(self.__current_message) == self.__current_expected_length:
                self.__current_expected_length = None
                self.__current_expected_length_bytes = b''

                message = self.__current_message.decode('utf-8')
                self.__current_message = None

                return message

    def __receive_length(self):
        self.__current_expected_length_bytes += self.__serial.read(1)

        if len(self.__current_expected_length_bytes) == 2:
            self.__current_expected_length = int.from_bytes(
                self.__current_expected_length_bytes, byteorder='little')
            self.__current_message = b''
