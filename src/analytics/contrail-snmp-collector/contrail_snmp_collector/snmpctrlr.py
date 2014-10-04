from snmp import SnmpSession
from snmpuve import SnmpUve
import time
import gevent

class Controller(object):
    def __init__(self, config):
        self._config = config
        self.uve = SnmpUve(self._config)
        self.sleep_time()
        self._keep_running = True

    def stop(self):
        self._keep_running = False

    def sleep_time(self, newtime=None):
        if newtime:
            self._sleep_time = newtime
        else:
            self._sleep_time = self._config.frequency()
        return self._sleep_time

    def task(self, netdev):
        ses = SnmpSession(netdev.snmp_cfg(), netdev.get_mibs())
        ses.scan_device(self.switcher)
        self.uve.send(ses.get_data())

    def switcher(self):
        gevent.sleep(0)

    def run(self):
        while self._keep_running:
            t = []
            for netdev in self._config.devices():
                t.append(gevent.spawn(self.task, netdev))
            gevent.joinall(t)
            del t
            time.sleep(self._sleep_time)
