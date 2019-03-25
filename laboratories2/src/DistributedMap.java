import org.jgroups.*;
import java.util.HashMap;
import java.util.Map;


public class DistributedMap implements SimpleStringMap {
    private HashMap<String, Integer> localMap = new HashMap<>();
    private JChannel channel;

    public DistributedMap(String name) throws Exception {
//        channel = JChannelProvider.createChannel();
        channel = JChannelProvider.createChannelSetIP("230.100.200.19");
        channel.setReceiver(new ExtendedReceiverAdapter(channel,this));
        channel.connect(name);
        channel.getState(null, 0);
    }

    @Override
    public void put(String key, Integer value) {
        try {
            channel.send(new Message(null, null, new Event(Event.State.PUT, key, value).toString()));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public Integer remove(String key) {
        try {
            channel.send(new Message(null, null, new Event(Event.State.REMOVE, key).toString()));
        } catch (Exception e) {
            e.printStackTrace();
        }
        return localMap.remove(key);
    }

    @Override
    public Integer get(String key) {
        return localMap.get(key);
    }

    @Override
    public boolean containsKey(String key) {
        return localMap.containsKey(key);
    }

    public HashMap<String, Integer> getLocalMap() {
        return localMap;
    }

    public void setLocalMap(Map<String, Integer> newState) {
        localMap.clear();
        localMap.putAll(newState);
    }
}

