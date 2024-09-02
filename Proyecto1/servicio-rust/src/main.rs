use std::fs::File;
use std::io::{self, Read};
use std::path::Path;
use chrono::{DateTime, Local};
use serde::{Deserialize, Serialize};


// CREACIÓN DE STRUCTS
#[derive(Debug, Serialize, Deserialize)]
struct SystemInfo {
    #[serde(rename = "Processes")]
    processes: Vec<Process>
}

#[derive(Debug, Serialize, Deserialize, Clone, PartialEq)]
struct Process {
    #[serde(rename = "PID")]
    pid: u32,
    #[serde(rename = "Name")]
    name: String,
    #[serde(rename = "Cmdline")]
    cmd_line: String,
    #[serde(rename = "MemoryUsage")]
    memory_usage: f64,
    #[serde(rename = "CPUUsage")]
    cpu_usage: f64,
}

#[derive(Debug, Serialize, Clone)]
struct LogProcess {
    pid: u32,
    container_id: String,
    name: String,
    memory_usage: f64,
    cpu_usage: f64,
}

#[derive(Debug, Serialize)]
struct AnalyzerOutput {
    current_time: String,
    killed_containers: Vec<LogProcess>,
    high_consumption_containers: Vec<Process>,
    low_consumption_containers: Vec<Process>,
    memory_info: MemoryInfo,
}

#[derive(Debug, Serialize, Deserialize)]
struct MemoryInfo {
    total_ram_kb: u64,
    free_ram_kb: u64,
    in_use_ram_kb: u64,
}

impl Process {
    fn get_container_id(&self) -> &str {
        let parts: Vec<&str> = self.cmd_line.split_whitespace().collect();
        for (i, part) in parts.iter().enumerate() {
            if *part == "-id" {
                if let Some(id) = parts.get(i + 1) {
                    return id;
                }
            }
        }
        "N/A"
    }

    fn get_image_name(&self) -> &str {
        let parts: Vec<&str> = self.cmd_line.split_whitespace().collect();
        for (i, part) in parts.iter().enumerate() {
            if *part == "-image" {
                if let Some(image) = parts.get(i + 1) {
                    return image;
                }
            }
        }
        "N/A"
    }
}

impl Eq for Process {}

impl Ord for Process {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.cpu_usage.partial_cmp(&other.cpu_usage).unwrap_or(std::cmp::Ordering::Equal)
            .then_with(|| self.memory_usage.partial_cmp(&other.memory_usage).unwrap_or(std::cmp::Ordering::Equal))
    }
}

impl PartialOrd for Process {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

fn kill_container(id: &str) -> Result<std::process::Output, io::Error> {
    println!("Matando contenedor con id: {}", id);
    let output = std::process::Command::new("sudo")
        .arg("docker")
        .arg("stop")
        .arg(id)
        .output()?;

    Ok(output)
}

fn print_current_time() -> String {
    let now: DateTime<Local> = Local::now();
    now.to_rfc3339()
}

fn analyzer(system_info: SystemInfo) -> AnalyzerOutput {
    let mut log_proc_list: Vec<LogProcess> = Vec::new();
    let mut processes_list: Vec<Process> = system_info.processes;

    // Filtramos el contenedor 'servidor-log_registry'
    let special_container_id = "servidor-log_registry";
    processes_list.retain(|process| process.get_image_name() != special_container_id);

    // Ordenar la lista de procesos por consumo (primero por CPU y luego por memoria)
    processes_list.sort_by(|a, b| {
        b.cpu_usage
            .partial_cmp(&a.cpu_usage)
            .unwrap_or(std::cmp::Ordering::Equal)
            .then_with(|| b.memory_usage.partial_cmp(&a.memory_usage).unwrap_or(std::cmp::Ordering::Equal))
    });

    // Obtener los 3 procesos de menor consumo
    let lowest_list: Vec<Process> = processes_list.iter().rev().take(3).map(|p| p.clone()).collect();

    // Obtener los 2 procesos de mayor consumo
    let highest_list: Vec<Process> = processes_list.iter().take(2).map(|p| p.clone()).collect();

    // Evitar eliminar contenedores cuyo ID coincide con los primeros 12 caracteres de "5d824b2fbe94"
    let skip_id_prefix = "5d824b2fbe94";

    // Preparar lista para eliminar contenedores
    let mut containers_to_kill: Vec<Process> = Vec::new();

    for process in processes_list.iter() {
        let container_id = process.get_container_id().to_string();
        
        // Saltar si está en las listas de bajo o alto consumo
        if lowest_list.contains(process) || highest_list.contains(process) {
            continue;
        }

        // Saltar si el ID del contenedor coincide con el prefijo que queremos evitar
        if container_id.starts_with(skip_id_prefix) {
            continue;
        }

        containers_to_kill.push(process.clone());
    }

    // Asegúrate de que no se eliminen más de 5 contenedores
    let max_to_kill = 5;
    if containers_to_kill.len() > max_to_kill {
        containers_to_kill.truncate(max_to_kill);
    }

    // Ahora matamos los contenedores
    for process in containers_to_kill.iter() {
        let container_id = process.get_container_id().to_string();
        let log_process = LogProcess {
            pid: process.pid,
            container_id: container_id.clone(),
            name: process.name.clone(),
            memory_usage: process.memory_usage,
            cpu_usage: process.cpu_usage,
        };
        log_proc_list.push(log_process.clone());

        if let Err(e) = kill_container(&container_id) {
            eprintln!("Error al matar el contenedor: {}", e);
        }
    }

    // Leer la información de memoria
    let memory_info = read_memory_info().unwrap_or_else(|_| MemoryInfo {
        total_ram_kb: 0,
        free_ram_kb: 0,
        in_use_ram_kb: 0,
    });

    // Crear salida en JSON
    let output = AnalyzerOutput {
        current_time: print_current_time(),
        killed_containers: log_proc_list,
        high_consumption_containers: highest_list,
        low_consumption_containers: lowest_list,
        memory_info,
    };

    output
}

fn read_proc_file(file_name: &str) -> io::Result<String> {
    let path = Path::new("/proc").join(file_name);
    let mut file = File::open(path)?;
    let mut content = String::new();
    file.read_to_string(&mut content)?;
    Ok(content)
}

fn read_memory_info() -> io::Result<MemoryInfo> {
    let memory_file_content = read_proc_file("sysinfo_201902416")?;
    let json_value: serde_json::Value = serde_json::from_str(&memory_file_content)
        .map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?;

    // Extraer la información de memoria del JSON
    let memory_info = json_value.get("Memory")
        .ok_or_else(|| io::Error::new(io::ErrorKind::NotFound, "Memory section not found"))?
        .as_object()
        .ok_or_else(|| io::Error::new(io::ErrorKind::InvalidData, "Memory section is not an object"))?;

    let total_ram_kb = memory_info.get("Total RAM(KB)")
        .and_then(|v| v.as_u64())
        .ok_or_else(|| io::Error::new(io::ErrorKind::InvalidData, "Total RAM(KB) not found or invalid"))?;

    let free_ram_kb = memory_info.get("Free RAM(KB)")
        .and_then(|v| v.as_u64())
        .ok_or_else(|| io::Error::new(io::ErrorKind::InvalidData, "Free RAM(KB) not found or invalid"))?;

    let in_use_ram_kb = memory_info.get("In Use RAM(KB)")
        .and_then(|v| v.as_u64())
        .ok_or_else(|| io::Error::new(io::ErrorKind::InvalidData, "In Use RAM(KB) not found or invalid"))?;

    Ok(MemoryInfo {
        total_ram_kb,
        free_ram_kb,
        in_use_ram_kb,
    })
}

fn parse_proc_to_struct(json_str: &str) -> Result<SystemInfo, serde_json::Error> {
    let system_info: SystemInfo = serde_json::from_str(json_str)?;
    Ok(system_info)
}

use reqwest::blocking::Client;
fn send_json_to_server(json_data: &AnalyzerOutput) -> Result<(), reqwest::Error> {
    let client = Client::new();
    let res = client.post("http://localhost:8000/logs")
        .json(json_data)
        .send()?;

    if res.status().is_success() {
        println!("JSON enviado exitosamente.");
    } else {
        println!("Error al enviar JSON: {}", res.status());
    }

    Ok(())
}


fn main() {
    loop {
        let system_info: Result<SystemInfo, _>;
        let json_str = read_proc_file("sysinfo_201902416").unwrap();
        system_info = parse_proc_to_struct(&json_str);

        match system_info {
            Ok(info) => {
                let output = analyzer(info);
                // Imprimir salida en JSON
                println!("{}", serde_json::to_string_pretty(&output).unwrap());

                // Enviar JSON al servidor HTTP
                if let Err(e) = send_json_to_server(&output) {
                    eprintln!("Error al enviar JSON: {}", e);
                }
            }
            Err(e) => println!("Failed to parse JSON: {}", e),
        }

        std::thread::sleep(std::time::Duration::from_secs(10));
    }
}