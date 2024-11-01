
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
kubectl rollout restart deployment  -n redis


--Ejecutar redis
kubectl exec -it redis-57b4665c96-bh9jf -n redis -- /bin/sh

-----------------------------
kubectl get pods -n nginx-ingress
kl nginx-ingress-ingress-nginx-controller-6ffbff94df-2f6x6 -n nginx-ingress
kubectl describe ingress sopes1-ingress -n sopes1



ka kafka-consumer-winners.yaml -n sopes1 
ka kafka-consumer-loser.yaml -n sopes1 

kubectl rollout restart deployment kafka-consumer-go-winners -n sopes1
kubectl rollout restart deployment kafka-consumer-go-losers -n sopes1









kubectl exec -it redis-7f64fc6ddf-5pkkz -n redis -- /bin/sh

--
Ejecutar grafana
kubectl port-forward service/my-cluster-kafka-bootstrap -n kafka 9092:9092






----

Configuracion para el ingress

1. **Crear el espacio de nombres y agregar el repositorio de NGINX Ingress en Helm**:

   ```bash
   kubectl create ns nginx-ingress
   helm repo add ingress-nginx https://kubernetes.github.io/ingress-nginx
   helm repo update
   ```

2. **Instalar el controlador de Ingress de NGINX**:

   ```bash
   helm install nginx-ingress ingress-nginx/ingress-nginx -n nginx-ingress
   ```

3. **Verificar los servicios y obtener la IP externa del Load Balancer**:

   Ejecuta este comando para ver los servicios en el espacio de nombres `nginx-ingress`:

   ```bash
   kubectl get services -n nginx-ingress
   ```

   Busca el servicio con el tipo `LoadBalancer`. La columna `EXTERNAL-IP` mostrará la IP pública asignada. Si ves `<pending>`, espera unos segundos y vuelve a ejecutar el comando hasta que aparezca la IP.

4. **Modificar tu archivo `ingress.yaml`**:
   
   Abre el archivo `ingress.yaml` y usa la IP que obtuviste en `spec.rules.host`. El bloque `ingress.yaml` debería verse algo así:

   ```yaml
   apiVersion: networking.k8s.io/v1
   kind: Ingress
   metadata:
     name: example-ingress
     namespace: default
   spec:
     rules:
       - host: "TU_IP_O_DOMINIO"
         http:
           paths:
             - path: /
               pathType: Prefix
               backend:
                 service:
                   name: nombre-del-servicio
                   port:
                     number: 80
   ```

5. **Aplicar el archivo `ingress.yaml`**:

   Finalmente, despliega el Ingress con:

   ```bash
   kubectl apply -f ingress.yaml
   ```

Estos comandos deberían ayudarte a configurar y exponer tu aplicación a través del Ingress en GCP.






kubectl port-forward service/grafana -n grafana 3001:50060