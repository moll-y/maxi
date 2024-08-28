package main

import (
	"log"
	"net"
	"time"
)

func udpServer(cmds chan Command) {
	addr := net.UDPAddr{Port: 1053, IP: net.ParseIP("0.0.0.0")}
	pc, err := net.ListenUDP("udp", &addr)
	if err != nil {
		log.Fatal(err)
	}
	defer pc.Close()
	c := make(chan string)
	go (func() {
		for {
			buf := make([]byte, 1024)
			_, addr, err := pc.ReadFrom(buf)
			if err != nil {
				continue
			}
			go (func() {
				ip := addr.(*net.UDPAddr).IP.String()
				iss := buf[0]
				if iss != 0x01 {
					return
				}
				req := buf[1]
				if req == 0x00 {
					c <- ip
					return
				}
				if req != 0x01 {
					return
				}
				msg := buf[2]
				if msg == 0x00 {
					return
				}
				cmds <- Command{IP: ip, Payload: msg}
				c <- ip
			})()
		}
	})()
	ips := make(map[string]time.Time, 100)
	for {
		select {
		default:
			now := time.Now()
			for ip, v := range ips {
				if now.Sub(v) > 2*time.Second {
					log.Printf("device %s deleted\n", ip)
					delete(ips, ip)
				}
			}
			time.Sleep(time.Second)
		case ip := <-c:
			if _, ok := ips[ip]; !ok {
				log.Printf("device %s added\n", ip)
			}
			ips[ip] = time.Now()
		}
	}
}
