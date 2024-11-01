
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