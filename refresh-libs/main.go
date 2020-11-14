package main

import (
	"flag"
	"log"
	"os/exec"
)

func main() {
	refreshFlag := flag.String("refresh", "", "Refresh libs in the project")
	flag.Parse()
	
	if *refreshFlag!="" {
		refreshProject(*refreshFlag)
	}
}

func refreshProject(projectFolderPath string) {
	command := exec.Command("ant", "refresh-libs")
	command.Dir=projectFolderPath
	err := command.Run()
	if err != nil {
		log.Fatal(err)
	}
}
