public class SimpleApp {
    public static void main(String[] args) {
        System.out.println("Hello from the lightweight Java container!");
        
        // Mantener el contenedor en ejecución
        try {
            Thread.sleep(Long.MAX_VALUE); // Pausa muy larga (prácticamente infinita)
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

// javac SimpleApp.java
// jar cfe SimpleApp.jar SimpleApp SimpleApp.class
// docker build -t low-image-1 .
