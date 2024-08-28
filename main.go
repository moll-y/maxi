package main

import (
	"log"
)

func main() {
	quit := make(chan bool)
	cmds := make(chan Command, 100)
	go httpServer(cmds)
	go udpServer(cmds)
	go (func() {
		for {
			select {
			case cmd := <-cmds:
				log.Println(cmd)
			}
		}
	})()
	<-quit
}
