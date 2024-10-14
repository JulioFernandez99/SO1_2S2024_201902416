use warp::Filter;
use serde::{Deserialize, Serialize};
use std::sync::{Arc, Mutex};
use std::thread;

#[derive(Debug, Deserialize, Serialize, Clone)]  // Clonable para poder clonar el estudiante
struct Student {
    student: String,
    age: i32,
    faculty: String,
    discipline: i32,
}

// Función para obtener el nombre de la disciplina según el valor
fn get_discipline_name(discipline: i32) -> &'static str {
    match discipline {
        1 => "Natación",
        2 => "Atletismo",
        3 => "Boxeo",
        _ => "Desconocido",
    }
}

// Función para validar los datos del estudiante
fn validate_student(student: &Student) -> Result<(), String> {
    if student.age < 0 {
        return Err(format!("Edad inválida para el estudiante {}", student.student));
    }
    if student.discipline < 1 || student.discipline > 3 {
        return Err(format!("Disciplina inválida para el estudiante {}", student.student));
    }
    Ok(())
}

#[tokio::main]

async fn main() {
    // Prefijado con guion bajo para suprimir la advertencia
    let _students_data: Arc<Mutex<Vec<Student>>> = Arc::new(Mutex::new(Vec::new()));

    // Definir el filtro para manejar las solicitudes POST
    let students_route = warp::post()
        .and(warp::path("Ingenieria"))
        .and(warp::body::json())
        .map({
            move |students: Vec<Student>| {  // Mover la variable students al closure
                let mut handles = vec![];

                for student in students.clone() {  // Clonamos estudiantes aquí
                    let discipline_name = get_discipline_name(student.discipline);
                    
                    // Lanzar un nuevo thread para procesar el estudiante
                    let student_clone = student.clone();  // Clonamos el estudiante
                    let handle = thread::spawn(move || {
                        // Validar el estudiante
                        if let Err(e) = validate_student(&student_clone) {
                            eprintln!("Error de validación para el estudiante {}: {}", student_clone.student, e);
                            return;
                        }

                        println!("Redirigiendo al estudiante {} a la disciplina {}", student_clone.student, discipline_name);

                        // Simular el envío a la disciplina
                        println!("Simulando el envío de {} a la disciplina {}", student_clone.student, discipline_name);

                        // Aquí puedes agregar lógica para enviar al servidor gRPC
                    });

                    handles.push(handle);
                }

                // Esperar a que todos los threads terminen
                for handle in handles {
                    let _ = handle.join();
                }

                warp::reply::json(&students) // Retornar los estudiantes como respuesta
            }
        });

    let prueba_route = warp::path("Prueba-ingenieria")
        .and(warp::get())  // Asegúrate de que acepte GET
        .map(|| {
            warp::reply::json(&"¡Ruta de prueba de ingeniería alcanzada!")
        });

    let _routes = students_route.or(prueba_route);
    // Iniciar el servidor en el puerto 8000
    warp::serve(_routes)
        .run(([0, 0, 0, 0], 8000))
        .await;
}
