from .monitor import SerialMonitor

if __name__ == '__main__':
    with SerialMonitor() as m:
        m.run()
