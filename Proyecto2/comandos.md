
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








----
gcloud config set project py2-sopes1
gcloud container clusters get-credentials sopes1 --zone us-east1-b



---- Comandos gcp
ka namesapce.yaml
ka ingress.yaml
ka goclient.yaml -n sopes1
ka rustclient.yaml -n sopes1
ka goserverAtletismo.yaml -n sopes1
ka goserverNatacion.yaml  -n sopes1
ka goserverBoxeo.yaml -n sopes1
kubectl create namespace kafka
kubectl apply -f https://strimzi.io/install/latest?namespace=kafka -n kafka
kubectl apply -f https://strimzi.io/examples/latest/kafka/kraft/kafka-single-node.yaml -n kafka
kubectl apply -f kafka-topic.yaml -n kafka
ka kafka-consumer-winners.yaml -n sopes1 
ka kafka-consumer-loser.yaml -n sopes1 

go get github.com/go-redis/redis/v8
kubectl create namespace redis
ka redis.yaml -n redis
kubectl get pods -n redis

kubectl create namespace grafana
kubectl apply -f grafana.yaml -n grafana

--
kubectl rollout restart deployment redis -n redis


--Ejecutar redis
kubectl exec -it redis-7fcf4f6777-2z7hx -n redis -- /bin/sh
