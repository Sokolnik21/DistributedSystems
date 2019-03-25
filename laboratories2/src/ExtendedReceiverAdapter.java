import org.jgroups.*;
import org.jgroups.util.Util;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Vector;

public class ExtendedReceiverAdapter extends ReceiverAdapter {
    private final DistributedMap state;
    private final JChannel channel;

    ExtendedReceiverAdapter(JChannel channel, DistributedMap map) {
        this.channel = channel;
        this.state = map;
    }

    @Override
    public void receive(Message msg) {
        System.out.println("Received message from " + msg.getSrc() + ": " + msg.getObject().toString());

        Event e = null;
        try {
            e = new Event(msg.getObject().toString());
        } catch (Exception e1) {
            e1.printStackTrace();
        }

        switch(e.getState()) {
            case PUT :
                state.getLocalMap().put(e.getKey(), e.getValue());
                break;
            case REMOVE :
                state.getLocalMap().remove(e.getKey());
        }
    }

    @Override
    public void getState(OutputStream output) throws Exception {
        synchronized (state) {
            Util.objectToStream(state.getLocalMap(), new DataOutputStream(output));
        }
    }

    @Override
    public void setState(InputStream input) throws Exception {
        synchronized (state) {
            state.setLocalMap((HashMap<String, Integer>) Util.objectFromStream(new DataInputStream(input)));
        }
    }

    @Override
    public void viewAccepted(View view) {
        System.out.println(view.toString());
        handleView(channel, view);
    }

    private static void handleView(JChannel ch, View new_view) {
        if(new_view instanceof MergeView) {
            ViewHandler handler = new ViewHandler(ch, (MergeView)new_view);
            // requires separate thread as we don't want to block JGroups
            handler.start();
        }
    }

    private static class ViewHandler extends Thread {
        JChannel ch;
        MergeView view;

        private ViewHandler(JChannel ch, MergeView view) {
            this.ch = ch;
            this.view = view;
        }

        public void run() {
            Vector<View> subgroups = (Vector<View>) view.getSubgroups();
            View tmp_view = subgroups.firstElement(); // picks the first
            Address local_addr = ch.getAddress();
            if(!tmp_view.getMembers().contains(local_addr)) {
                System.out.println("Not member of the new primary partition ("
                        + tmp_view + "), will re-acquire the state");
                try {
                    ch.getState(null, 30000);
                }
                catch(Exception ex) {
                }
            }
            else {
                System.out.println("Not member of the new primary partition ("
                        + tmp_view + "), will do nothing");
            }
        }
    }
}