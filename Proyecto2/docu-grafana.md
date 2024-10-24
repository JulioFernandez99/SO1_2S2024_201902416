Aquí te dejo el archivo completo para desplegar Grafana en un namespace llamado `grafana`, junto con los recursos necesarios.

### Paso 1: Crear el namespace para Grafana
Ejecuta este comando para crear el namespace `grafana`:

```bash
kubectl create namespace grafana
```

### Paso 2: Desplegar Grafana
Guarda el siguiente archivo YAML como `grafana-deployment.yaml`, luego aplícalo con `kubectl` para crear el deployment y el servicio.

```yaml
apiVersion: v1
kind: Namespace
metadata:
  name: grafana

---
apiVersion: v1
kind: Service
metadata:
  name: grafana
  namespace: grafana
  labels:
    app: grafana
spec:
  type: NodePort
  ports:
    - port: 3000
      nodePort: 50060 # Especifica el puerto externo aquí
      targetPort: 3000
  selector:
    app: grafana

---
apiVersion: apps/v1
kind: Deployment
metadata:
  name: grafana
  namespace: grafana
spec:
  replicas: 1
  selector:
    matchLabels:
      app: grafana
  template:
    metadata:
      labels:
        app: grafana
    spec:
      containers:
        - name: grafana
          image: grafana/grafana:latest
          ports:
            - containerPort: 3000
          env:
            - name: GF_SECURITY_ADMIN_USER
              value: "admin"
            - name: GF_SECURITY_ADMIN_PASSWORD
              value: "admin"
```

### Paso 3: Aplicar el archivo de deployment
Aplica el archivo `grafana-deployment.yaml` ejecutando el siguiente comando:

```bash
kubectl apply -f grafana-deployment.yaml
```

### Explicación de las variables:
- **GF_SECURITY_ADMIN_USER**: Nombre de usuario administrador para Grafana. Puedes cambiarlo si deseas un nombre de usuario diferente.
- **GF_SECURITY_ADMIN_PASSWORD**: Contraseña para el usuario administrador. En este caso, está configurada como `admin`, pero debes cambiarla por algo más seguro en un entorno de producción.

### Verificar el deployment
Puedes verificar si el pod de Grafana está en ejecución utilizando:

```bash
kubectl get pods -n grafana
```

### Acceder a Grafana
Accede a Grafana en tu navegador en `http://<node-ip>:50060`. El puerto `50060` se asigna como un `NodePort`, lo que permite que Grafana sea accesible desde fuera del clúster. Asegúrate de reemplazar `<node-ip>` con la IP del nodo donde está ejecutándose Grafana.


kubectl port-forward -n grafana grafana-67c647f54c-8dnks 50060:3000
Address: redis-service.redis:6379
