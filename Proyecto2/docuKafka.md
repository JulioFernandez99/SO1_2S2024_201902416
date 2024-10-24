
### 1. Crear el namespace para Kafka
Primero, debes crear un namespace para organizar los recursos de Kafka:

```bash
kubectl create namespace kafka
```

### 2. Desplegar Strimzi (el operador de Kafka)
Strimzi es un operador que facilita la gestión y despliegue de Kafka en Kubernetes. Para desplegar Strimzi en el namespace `kafka`, puedes usar el siguiente comando:

```bash
kubectl apply -f https://strimzi.io/install/latest?namespace=kafka -n kafka
```

Este comando descargará y aplicará los recursos necesarios para el operador Strimzi en tu clúster de Kubernetes.

### 3. Desplegar el clúster de Kafka con Strimzi
Una vez que Strimzi esté desplegado, puedes desplegar un clúster de Kafka. Estás utilizando una configuración para un nodo único con Kraft (que elimina la necesidad de ZooKeeper). Para esto, puedes ejecutar:

```bash
kubectl apply -f https://strimzi.io/examples/latest/kafka/kraft/kafka-single-node.yaml -n kafka
```

Este YAML desplegará un clúster Kafka básico de un solo nodo. Luego, puedes usar este comando para asegurarte de que el clúster Kafka esté listo antes de continuar:

```bash
kubectl wait kafka/my-cluster --for=condition=Ready --timeout=300s -n kafka
```

### 4. Crear los tópicos de Kafka
Luego de que el clúster esté listo, puedes crear los tópicos `winners` y `losers`. El archivo `kafka-topics.yaml` que has definido parece correcto, y para aplicarlo puedes guardarlo en un archivo `kafka-topics.yaml` y luego ejecutar el siguiente comando:

```bash
kubectl apply -f kafka-topics.yaml -n kafka
```

Aquí está tu archivo `kafka-topics.yaml`:

```yaml
apiVersion: kafka.strimzi.io/v1beta2
kind: KafkaTopic
metadata:
  name: winners  # Nombre del tópico "winners"
  namespace: kafka  # El namespace donde estás desplegando Kafka
  labels:
    strimzi.io/cluster: my-cluster  # El cluster de Kafka con el que se relaciona
spec:
  partitions: 1  # Número de particiones
  replicas: 1    # Número de réplicas

---
apiVersion: kafka.strimzi.io/v1beta2
kind: KafkaTopic
metadata:
  name: losers  # Nombre del tópico "losers"
  namespace: kafka  # El namespace donde estás desplegando Kafka
  labels:
    strimzi.io/cluster: my-cluster  # El cluster de Kafka con el que se relaciona
spec:
  partitions: 1  # Número de particiones
  replicas: 1    # Número de réplicas
```

### Pasos adicionales
- **Verificar los pods de Kafka**: Puedes asegurarte de que los pods del clúster de Kafka estén corriendo correctamente con:

   ```bash
   kubectl get pods -n kafka
   ```

- **Comprobar los tópicos**: Una vez aplicados los YAMLs de los tópicos, puedes verificar su creación usando:

   ```bash
   kubectl get kafkatopic -n kafka
   ```

