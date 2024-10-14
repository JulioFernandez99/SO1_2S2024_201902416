# Proyecto 2 Sistemas operativos

## Configuracion de los deployments iniciales
Estos deployments recibiran las peticiones de ingenieria y agronomia, desde el ingress creado,
Un deployment puede contener pods y servicios, los servicios son configuraciones que nos ayudaran para conectarnos con varios pods, sin tener que especificarlos.
iniciemos creando un Deployment:
```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: ingenieria-deploy
spec:
  selector:
    matchLabels:
      app: ingenieria-app
  template:
    metadata:
      labels:
        app: ingenieria-app
    spec:
      containers:
      - name: ingenieria-app
        image: nginxdemos/hello:latest
        resources:
          limits:
            memory: "128Mi"
            cpu: "500m"
        ports:
        - containerPort: 8000

---
apiVersion: apps/v1
kind: Deployment
metadata:
  name: agronomia-deploy
spec:
  selector:
    matchLabels:
      app: agronomia-app
  template:
    metadata:
      labels:
        app: agronomia-app
    spec:
      containers:
      - name: agronomia-app
        image: nginxdemos/hello:latest
        resources:
          limits:
            memory: "128Mi"
            cpu: "500m"
        ports:
        - containerPort: 8000
```
Aca tenemos dos deployments, cada uno de ellos va tener un pod de momento tienen como imagen para crear el pod, hello, esta imagen solamente realizara un saludo, el nombre del pod varia con respecto al deployment, en caso de ingenieria vamos a tener ingenieria-app, y en los pods vamos a ingresar por el puerto 8000

## Configuracion de servicios
Los servicios son configuraciones que realizamos y creamos para poder comunicarnos con varios pods, sin tener que especificar un puerto, el problema es que cuando se crea un pod, kubernetes le asigna un puerto automatico, siendo esto desde 300000 en adelante creo, siempre siendo diferente para cada pod. Nosotros nos debemos conectar por el puerto 8000, al puerto que kubernetes le asigne.

```yaml
apiVersion: v1
kind: Service
metadata:
  name: ingenieria-service
spec:
  type: NodePort
  selector:
    app: ingenieria-app
  ports:
  - port: 8000
    targetPort: 80

---
apiVersion: v1
kind: Service
metadata:
  name: agronomia-service
spec:
  type: NodePort
  selector:
    app: agronomia-app
  ports:
  - port: 8000
    targetPort: 80
```
En este servicio le decimos que se conecte el servicio de ingenieria a los pod de ingenieria con nombre ingenieria-app.

Dentro del pod, el se levanta una aplicacion, para poder llegar a el servicio que va saludar, vamos a usar el protocolo http, esto por el puerto 80 ya que no nos impone restriccion ni firewall.

Ingress  -> servicio -> deployment -> pod:8000-> puerto asignado kubernetes -> 80

esta es aproximadamente la ruta que se tomara para poder llegar al saludo realizado por el pod.

## Configurando el Ingress
el ingress funciona parecido a un end point, siempre siguen siendo configuraciones, estas reciben las rutas que vienen desde cualquier lugar y las filtran y redirigen a diferentes pods:
```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: proyecto-ingres
  labels:
    name: ProyectoIngres
  annotations:
    nginx.ingress.kubernetes.io/rewrite-target: /
spec:
  rules:
  - host: "hello-world.example"
    http:
      paths:
      - path: "/Ingenieria"
        pathType: Prefix
        backend:
          service:
            name: ingenieria-service
            port:
              number: 8000
      - path: "/Agronomia"
        pathType: Prefix
        backend:
          service:
            name: agronomia-service
            port:
              number: 8000
```
en este ingres tengo dos configuraciones, si ingresa http://hello-world.example/Ingenieria me redirige a el servicio con nombre ingenieria-service, de igual forma si ingresa Agronomia. de esta forma podemos redirigir las peticiones que ingresen en nuestro cluster

## Notas

Para GCP no tengo la configuracion esto es mas el local, pero si funciona en local crea que son pocas las configuraciones que se deben realizar.

En local utilice Minikube, tienen que tener en cuenta que en local hello-world.com no funcionara ya que no es un dns registrado, esto lo tendran que hacer manual, para ello deben realizar lo siguiente

Obtenemos la ip que tiene asignada minikube
```bash
minikube ip //ejemplo: 192.168.49.2
```
esta ip tendrian que colocarla en vez de hello-world.com en el ingres, pero este no lo reconocera, por ello debemos registrar la ip en los hosts del sistema, para que logre reconocer la dns deben agregarla a los hosts.

En linux tenemos la carpeta etc que guarda los atributos de usuario basicos, ejemplo los hosts y las variables de entorno global.

Para guardar el dns debemos agregarlo al archivo hosts en etc

```
sudo vim /etc/hosts
```
en este comando usaremo vim para agregar al archivo lo siguiente

ipdeminikube hello-world.com

con ello ya podremos conectar con los servicios y por ende a los pds.

para iniciar el ingress -> minikube addons enable ingress