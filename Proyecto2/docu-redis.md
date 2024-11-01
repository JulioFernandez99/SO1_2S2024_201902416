### Instalar redis
go get github.com/go-redis/redis/v8

kubectl create namespace redis


### 1. Acceder al Pod de Redis

Primero, debes acceder al pod de Redis en tu clúster de Kubernetes. Puedes usar `kubectl` para obtener la lista de pods en el namespace donde está Redis (en este caso, `redis`).

```bash
kubectl get pods -n redis
```



Busca el nombre del pod de Redis en la salida. Luego, accede al pod:

```bash
kubectl exec -it redis-7f64fc6ddf-5pkkz -n redis -- /bin/sh
```

Reemplaza `<nombre-del-pod>` con el nombre real de tu pod de Redis.

### 2. Usar la CLI de Redis

Una vez que estés dentro del pod de Redis, puedes utilizar la CLI de Redis para verificar tus datos.

1. **Iniciar el cliente de Redis**:
   ```sh
   redis-cli
   ```

2. **Verifica los datos**:
   Usa el comando `LRANGE` para obtener los elementos de la lista `students`:

   ```sh
   LRANGE students 0 -1
   ```

   Esto te mostrará todos los elementos almacenados en la lista `students`.

### 3. Salir del Pod

Después de verificar tus datos, puedes salir del cliente de Redis y luego del pod:

- Para salir del cliente de Redis, simplemente escribe:
  ```sh
  exit
  ```

- Luego, para salir del pod:
  ```sh
  exit
  ```

### Resumen

Con estos pasos, podrás acceder al pod de Redis en Kubernetes y verificar la información almacenada utilizando la CLI de Redis. Si encuentras algún problema o necesitas ayuda adicional, no dudes en preguntar.