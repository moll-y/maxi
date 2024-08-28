package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
)

func main() {
	conn, err := net.Dial("udp", "0.0.0.0:1053")
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()
	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("Enter command: ")
		scanner.Scan()
		txt := scanner.Text()
		var msg byte
		switch txt {
		case "a":
			msg = 0x01
		case "b":
			msg = 0x02
		case "c":
			msg = 0x03
		case "d":
			msg = 0x04
		}
		_, err = conn.Write([]byte{0x01, 0x00, msg})
		if err != nil {
			log.Fatal(err)
		}
	}
}
