#!/usr/bin/env python3
import os
import curses
from curses import panel
from subprocess import Popen, PIPE, call

path_tcp_client = os.path.abspath("./client/tcp/Client")
path_tcp_server = os.path.abspath("./server/tcp/Server")

path_tcp2_client = os.path.abspath("./client/tcp2/Client")
path_tcp2_server = os.path.abspath("./server/tcp2/Server")

path_udp_server = os.path.abspath("./server/udp/Server")
path_udp_client = os.path.abspath("./client/udp/Client")

path_udp_tcp_server = os.path.abspath("./server/udptcp/Server")
path_udp_tcp_client = os.path.abspath("./client/udptcp/Client")

port = "6666"
ip = "127.0.0.1"

class Menu(object):

    def __init__(self, items, stdscreen):
        self.window = stdscreen.subwin(0,0)
        self.window.keypad(1)
        self.panel = panel.new_panel(self.window)
        self.panel.hide()
        panel.update_panels()

        self.position = 0
        self.items = items
        self.items.append(('exit','exit'))

    def navigate(self, n):
        self.position += n
        if self.position < 0:
            self.position = 0
        elif self.position >= len(self.items):
            self.position = len(self.items)-1

    def display(self):
        self.panel.top()
        self.panel.show()
        self.window.clear()

        while True:
            self.window.refresh()
            curses.doupdate()
            for index, item in enumerate(self.items):
                if index == self.position:
                    mode = curses.A_REVERSE
                else:
                    mode = curses.A_NORMAL

                msg = '%d. %s' % (index, item[0])
                self.window.addstr(1+index, 1, msg, mode)

            key = self.window.getch()

            if key in [curses.KEY_ENTER, ord('\n')]:
                if self.position == len(self.items)-1:
                    break
                else:
                    self.items[self.position][1]()

            elif key == curses.KEY_UP:
                self.navigate(-1)

            elif key == curses.KEY_DOWN:
                self.navigate(1)

        self.window.clear()
        self.panel.hide()
        panel.update_panels()
        curses.doupdate()

class MyApp(object):

    def __init__(self, stdscreen):
        self.screen = stdscreen
        curses.curs_set(0)
        submenu_udp = [ 
                 ('Start Server',self.run_udp_server),
                ('Start Client', self.run_udp_client),
                ('About',curses.beep)
        ]
        submenu_udp = Menu(submenu_udp,self.screen)
       
        submenu_tcp = [
            ('Start Server with file transfer', self.run_tcp_server_f),
            ('Start Client with file transfer', self.run_tcp_client_f),
            ('Start Server',self.run_tcp_server),
            ('Start Client', self.run_tcp_client),
            ('About', curses.beep)
        ]
        submenu_tcp = Menu(submenu_tcp,self.screen)
       
        submenu_items = [
                ('beep', curses.beep),
                ('flash', curses.flash)
                ]
        submenu = Menu(submenu_items, self.screen)
        
        submenu_udp_tcp = [
                ('UDP/TCP Server', self.run_udp_tcp_s),
                ('UDP/TCP Client', self.run_udp_tcp_c)
        ]
        submenu_udp_tcp = Menu(submenu_udp_tcp, self.screen)
        main_menu_items = [
                ('About', submenu.display),
                ('TCP', submenu_tcp.display),
                ('UDP', submenu_udp.display),
                ('UDP and TCP', submenu_udp_tcp.display)
                ]
        main_menu = Menu(main_menu_items, self.screen)
        main_menu.display()
    def exit_curses(self):
        self.screen.clear()
        curses.nocbreak()
        self.screen.keypad(0)
        self.screen.move(0,0)
        curses.echo()
        curses.endwin()

    def enter_curses(self):
        curses.noecho()
        curses.cbreak()
        self.screen.keypad(1)


    def run_tcp_server(self):
       self.run_server(path_tcp2_server)
    def run_udp_server(self):
       self.run_server(path_udp_server)

    def run_tcp_client_f(self):
        self.run_tcp_client_file(path_tcp_client)
    def run_tcp_server_f(self):
        self.run_tcp_server_file(path_tcp_server)
    def run_tcp_client(self):
        self.run_client(path_tcp2_client)
    def run_tcp_server(self):
        self.run_server(path_tcp2_server)
   
    def run_udp_client(self):
       self.run_client(path_udp_client)
    def run_udp_tcp_s(self):
       self.run_udp_tcp_client(path_udp_tcp_server)
    def run_udp_tcp_c(self):
      self.run_udp_tcp_client(path_udp_tcp_client)
    
    def run_server(self, program):
       self.exit_curses()
       try:
            try:
               print program
               call([program, port])
            except ValueError, e:
                 print "Error opening file %s" % e
                 return
            except KeyboardInterrupt:
                 pass
       finally:
           self.enter_curses()
  
    def run_client(self, program):
        self.exit_curses()
        try:
            try:
                call([program, ip, port])
            except ValueError, e:
                print "Error opening file %s" % e
                return
            except KeyboardInterrupt:
                pass
        finally:
            self.enter_curses()
    def run_tcp_server_file(self, program):
        self.exit_curses()
        try:
            try:
                call([program,port])
            except ValueError, e:
                print "Error opening file %s" % e
                return
            except KeyboardInterrupt:
                pass
        finally:
            self.enter_curses()


    def run_tcp_client_file(self, program):
        self.exit_curses()
        try:
            try:
                call([program, ip,port])
            except ValueError, e:
                print "Error opening file %s" % e
                return
            except KeyboardInterrupt:
                pass
        finally:
            self.enter_curses()

    def run_udp_tcp_server(self, program):
        self.exit_curses()
        try:
            try:
                call([program, ip,port])
            except ValueError, e:
                print "Error opening file %s" % e
                return
            except KeyboardInterrupt:
                pass
        finally:
            self.enter_curses()
    def run_udp_tcp_client(self, program):
        self.exit_curses()
        try:
            try:
                call([program, ip, port])
            except ValueError, e:
                print "Error opening file %s" % e
                return
            except KeyboardInterrupt:
                pass
        finally:
            self.enter_curses()
if __name__ == '__main__':
    curses.wrapper(MyApp)
