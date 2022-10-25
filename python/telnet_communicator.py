from telnetlib import Telnet

if __name__ == "__main__":
    with Telnet("192.168.0.111", 80) as tn:
        print("Telnet stared")
        tn.interact()
        tn.write(b"hello")
