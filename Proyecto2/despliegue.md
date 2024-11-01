### Crear proyecto 
gcloud projects create p2-sopes-201902416
### Setear el proyecto
gcloud config set project p2-sopes-201902416

### Crear el cluster de k8s 
gcloud container clusters create proyecto2 --num-nodes=5 --region=us-west1-a --tags=allin,allout --machine-type=e2-medium --no-enable-network-policy --disk-size=25GB --disk-type pd-standard

## Obtener credenciales del cluster de k8s
gcloud container clusters get-credentials proyecto2 --zone us-west1-a

## Borrar cluster
gcloud container clusters delete proyecto2 --zone us-west1-a


El mensaje "No resources found in kube-system namespace" indica que no tienes instalado el controlador de Ingress de NGINX en tu clúster de Kubernetes. Aquí hay pasos que puedes seguir para instalarlo:

### Instalación del Controlador de Ingress de NGINX

1. **Agregar el repositorio de Helm** (si no lo has hecho antes):
   ```bash
   helm repo add ingress-nginx https://kubernetes.github.io/ingress-nginx
   helm repo update
   ```

2. **Instalar el controlador de NGINX**:
   ```bash
   helm install nginx-ingress ingress-nginx/ingress-nginx --namespace kube-system --create-namespace
   ```

   Este comando instalará el controlador de NGINX en el espacio de nombres `kube-system`.

3. **Verificar la instalación**:
   Después de unos momentos, verifica que los pods de NGINX estén corriendo:
   ```bash
   kubectl get pods -n kube-system -l app.kubernetes.io/name=ingress-nginx
   ```

4. **Configuración de Ingress**:
   - Asegúrate de que tu recurso `Ingress` esté correctamente configurado y apuntando a los servicios correctos, como lo has hecho en tu definición de `Ingress`.

5. **Comprobar la dirección IP**:
   Una vez que el controlador de Ingress esté funcionando, se te asignará una dirección IP externa. Puedes obtenerla con:
   ```bash
   kubectl get services -n kube-system -l app.kubernetes.io/name=ingress-nginx
   ```

6. **Actualizar tu recurso Ingress**:
   Si necesitas que tu `Ingress` utilice esta nueva configuración, asegúrate de que `ingressClassName` en tu recurso `Ingress` esté establecido en `nginx`.

### Uso del Controlador de GCP Ingress

Si prefieres usar el controlador de Ingress de Google en lugar de NGINX, puedes hacer lo siguiente:

1. **Modificar tu Ingress**:
   Cambia el `ingressClassName` en tu recurso `Ingress` a `gce`:

   ```yaml
   apiVersion: networking.k8s.io/v1
   kind: Ingress
   metadata:
     name: sopes1-ingress
     namespace: sopes1
     labels:
       name: sopes1-ingress
   spec:
     ingressClassName: gce
     rules: 
       - host: 36.231.190.222.nip.io
         http:
           paths:
             - pathType: Prefix
               backend:
                 service:
                   name: go-client-service
                   port:
                     number: 8080
               path: /Agronomia
             - pathType: Prefix
               backend:
                 service:
                   name: rust-client-service
                   port:
                     number: 8081
               path: /Ingenieria
   ```

2. **Aplicar el recurso**:
   Asegúrate de aplicar nuevamente tu recurso `Ingress`:

   ```bash
   kubectl apply -f tu-ingress.yaml
   ```

3. **Verificar la IP**:
   Luego, verifica la dirección IP externa asignada a tu `Ingress`:

   ```bash
   kubectl get ingress -n sopes1
   ```

Siguiendo estos pasos, deberías poder configurar correctamente tu controlador de Ingress en GCP. Si tienes alguna pregunta adicional o necesitas más ayuda, ¡déjamelo saber!














--------------



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