package main

import (
    "encoding/json"
    "fmt"
    "log"
    "net/http"
    "sync"
)

// Estructura que define a un estudiante
type Student struct {
    Student    string `json:"student"`
    Age        int    `json:"age"`
    Faculty    string `json:"faculty"`
    Discipline int    `json:"discipline"`
}

// Función para obtener el nombre de la disciplina según el valor
func getDisciplineName(discipline int) string {
    switch discipline {
    case 1:
        return "Natación"
    case 2:
        return "Atletismo"
    case 3:
        return "Boxeo"
    default:
        return "Desconocido"
    }
}

// Manejador de solicitudes HTTP
func handler(w http.ResponseWriter, r *http.Request) {
    var students []Student
    err := json.NewDecoder(r.Body).Decode(&students) // Decodificar el JSON recibido
    if err != nil {
        http.Error(w, err.Error(), http.StatusBadRequest)
        return
    }

    var wg sync.WaitGroup

    // Recorrer la lista de estudiantes y procesarlos usando goroutines
    for _, student := range students {
        wg.Add(1)
        go func(s Student) {
            defer wg.Done()

            // Validar datos del estudiante
            if err := validateStudent(s); err != nil {
                log.Printf("Error de validación para el estudiante %s: %v\n", s.Student, err)
                return
            }

            // Obtener el nombre de la disciplina
            disciplineName := getDisciplineName(s.Discipline)

            // Mensaje de depuración
            log.Printf("Redirigiendo al estudiante %s a la disciplina %s\n", s.Student, disciplineName)

            // Llamada a gRPC aquí para enviar al servidor de la disciplina correspondiente
            err := sendToDiscipline(s)
            if err != nil {
                log.Printf("Error al enviar al estudiante %s a la disciplina %s: %v\n", s.Student, disciplineName, err)
                // Aquí podrías enviar un mensaje de error al cliente si lo deseas
                return
            }

            // Crear una respuesta JSON
            response := map[string]string{
                "student":    s.Student,
                "discipline": disciplineName,
                "message":    "Enviado correctamente",
            }
            json.NewEncoder(w).Encode(response)
        }(student)
    }

    wg.Wait() // Esperar a que todas las goroutines terminen
}

// Función de validación de datos del estudiante
func validateStudent(s Student) error {
    if s.Age < 0 {
        return fmt.Errorf("edad inválida para el estudiante %s", s.Student)
    }
    if s.Discipline < 1 || s.Discipline > 3 {
        return fmt.Errorf("disciplina inválida para el estudiante %s", s.Student)
    }
    return nil
}

// Función que llamaría al servidor gRPC para redirigir al estudiante (simulación)
func sendToDiscipline(s Student) error {
    // Implementa la lógica de cliente gRPC aquí
    // Por ahora simulamos que la operación es exitosa
    log.Printf("Simulando el envío de %s a la disciplina %s\n", s.Student, getDisciplineName(s.Discipline))
    return nil
}

func pruebaHandler(w http.ResponseWriter, r *http.Request) {
    w.Write([]byte("Servidor Agronomía"))
}


func main() {
    http.HandleFunc("/Prueba-Agronomia", pruebaHandler)  
    http.HandleFunc("/Agronomia", handler) // Asignar la función manejadora a la ruta raíz
    log.Println("Servidor iniciado en :8000") // Mensaje de inicio del servidor
    log.Fatal(http.ListenAndServe(":8000", nil)) // Iniciar el servidor en el puerto 8080
}
