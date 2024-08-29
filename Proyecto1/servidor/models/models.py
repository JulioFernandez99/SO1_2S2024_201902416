from pydantic import BaseModel #type: ignore
from typing import List, Optional

class ContainerInfo(BaseModel):
    pid: int
    container_id: Optional[str]  # Optional en caso de que algunos campos sean opcionales
    name: str
    memory_usage: float
    cpu_usage: float

class ContainerHigh(BaseModel):
    PID: int
    Name: str
    Cmdline: str
    MemoryUsage: float
    CPUUsage: float

class MemoryInfo(BaseModel):
    total_ram_kb: int
    free_ram_kb: int
    in_use_ram_kb: int

class SystemLog(BaseModel):
    current_time: str
    killed_containers: List[ContainerInfo]
    high_consumption_containers: List[ContainerHigh]
    low_consumption_containers: List[ContainerHigh]
    memory_info: MemoryInfo
