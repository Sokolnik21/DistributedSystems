import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Main {
    public static void main(String[] args) throws Exception {
        System.setProperty("java.net.preferIPv4Stack","true");

        DistributedMap map = new DistributedMap("distributed_map");
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        Event e;

        while(true) {
            e = new Event(reader.readLine());
            switch(e.getState()) {
                case PUT :
                    map.put(e.getKey(), e.getValue());
                    break;
                case REMOVE :
                    System.out.println(map.remove(e.getKey()));
                    break;
                case GET :
                    System.out.println(map.get(e.getKey()));
                    break;
                case CONTAINS :
                    System.out.println(map.containsKey(e.getKey()));
                    break;
            }
        }
    }
}