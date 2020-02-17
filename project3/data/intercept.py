import dbus
#import dbus.service
#import dbus.mainloop.glib


#set up an event loop
from dbus.mainloop.glib import DBusGMainLoop
DBusGMainLoop(set_as_default=True)


def msg_handler(*args):
        print "session got hash "
        for arg in args:
                print arg + " //////////// "




bus = dbus.SessionBus()
bus.add_signal_receiver(msg_handler)


#start event loop
from gi.repository import GLib

loop = GLib.MainLoop()
loop.run()



