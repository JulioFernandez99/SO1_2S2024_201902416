package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

type Student struct {
	Student    string `json:"student"`
	Age        int    `json:"age"`
	Faculty    string `json:"faculty"`
	Discipline int    `json:"discipline"`
	Winner     int    `json:"winner"`
}

func handlePost(w http.ResponseWriter, r *http.Request) {
	// Leer el cuerpo de la solicitud
	body, err := ioutil.ReadAll(r.Body)
	if err != nil {
		http.Error(w, "Unable to read request body", http.StatusBadRequest)
		return
	}
	defer r.Body.Close()

	// Parsear el JSON
	var student Student
	err = json.Unmarshal(body, &student)
	if err != nil {
		http.Error(w, "Invalid JSON format", http.StatusBadRequest)
		return
	}

	// Logear los datos recibidos
	log.Printf("Received student data: %+v", student)

	// Enviar respuesta de éxito
	w.WriteHeader(http.StatusOK)
	w.Write([]byte("Received student data successfully"))
}

func main() {
	// Crear un servidor HTTP en el puerto 50055
	http.HandleFunc("/", handlePost)

	fmt.Println("HTTP server listening on port 50055")
	if err := http.ListenAndServe(":50055", nil); err != nil {
		log.Fatalf("Failed to start server: %v", err)
	}
}
