from .key import KBHit

class MessageReader:
    def __init__(self):
        self.__message = ""
        self.__kb = KBHit()
        self.__end_ch = None
        self.__ignore_ch = None

    def check_message(self):
        print(f"\33[2K\r[in]: {self.__message}", end="")

        if self.__kb.kbhit():
            ch = self.__kb.getch()

            if ch == '\r' or ch == '\n':
                if self.__end_ch is None:
                    self.__end_ch = ch
                    self.__ignore_ch = "\r" if ch == "\n" else "\n"

                if ch == self.__ignore_ch:
                    return None
                else:
                    message = self.__message
                    self.__message = ""
                    print()
                    return message

            if ord(ch) == 127 and len(self.__message) > 0:
                self.__message = self.__message[:-1]
            else:
                self.__message += ch

            print(f"\33[2K\r[in]: {self.__message}", end="")

        return None
