import java.math.BigInteger;

public class HeavyLoadApp {
    public static void main(String[] args) {
        System.out.println("Starting light load application...");
        while (true) {
            int sum = 0;
            for (int i = 0; i < 1000; i++) {
                sum += i;
            }
            System.out.println("Light load computation done.");
            try {
                Thread.sleep(1000); // Pausa más larga para reducir el uso de CPU
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}


// javac HeavyLoadApp.java
// jar cfe HeavyLoadApp.jar HeavyLoadApp HeavyLoadApp.class
// docker build -t heavy-java-app .
