
# Creacion de proyecto
```sh
gcloud projects create py2-sopes-201902416

```

# Establecer el proyecto actual
```sh
gcloud config set project proyecto2-sopes-2024
```

# Creacion de cluster
```sh
gcloud container clusters create sopes1 \
    --zone=us-central1-c \
    --num-nodes=1 \
    --tags=allin,allout \
    --machine-type=n1-standard-2 \
    --no-enable-network-policy
```

# Obtener credenciales del cluster
```sh
gcloud container clusters get-credentials sopes1 --zone us-central1-c --project py2-sopes-201902416
```


# Comandos extras

### Habilitar el ingress
```bash
minikube addons enable ingress
```

### Reiniciar un imagen en k8s
Este comando es util cuando se hace una modificacion en una imagen. Ya que al aplicar primeramente una imagen con un deployment luego de aplicarla descarga la imagen y esta no cambiara hasta ejecuar el comando ```kubectl rollout restart deployment <nombre-del-deployment>```

Por ejemplo:
```bash
kubectl rollout restart deployment grpc-server-natacion-go -n sopes1 -> actualiza el deployment cuando cambio una imagen en Dockerhub
```

ka kafka-consumer-winners.yaml -n sopes1 
ka kafka-consumer-loser.yaml -n sopes1 
kubectl rollout restart deployment kafka-consumer-go-winners -n sopes1
kubectl rollout restart deployment kafka-consumer-go-losers -n sopes1
kubectl rollout restart deployment redis -n redis


kubectl exec -it redis-57cd9b4cdf-vrwbl -n redis -- /bin/sh
redis-cli