import com.rabbitmq.client.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Administrator {
    public static void main(String[] argv) throws Exception {
        // info
        System.out.println("Administrator");

        // connection & channel
        Channel channel = ChannelProvider.configureChannel();

        // exchange
        String EXCHANGE_NAME = "Exchange";
        channel.exchangeDeclare(EXCHANGE_NAME, BuiltinExchangeType.TOPIC);

        // queue & bind
        String queueName = channel.queueDeclare().getQueue();
        channel.queueBind(queueName, EXCHANGE_NAME, "*.*.*");
        System.out.println("created queue: " + queueName);

        new Thread(new Runnable() {
            public void run()
            {
                // consumer (message handling)
                Consumer consumer = new DefaultConsumer(channel) {
                    @Override
                    public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body) throws IOException {
                        String message = new String(body, "UTF-8");
                        System.out.println(
                                envelope.getRoutingKey() + ";" +
                                message + ";" +
                                new SimpleDateFormat("yyyy/MM/dd HH:mm:ss").format(new Date()));

                        channel.basicAck(envelope.getDeliveryTag(), false);
                    }
                };

                // start listening
                System.out.println("Waiting for messages...");
                try {
                    channel.basicConsume(queueName, false, consumer);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }}).start();

        while (true) {

            // read msg
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            System.out.println("Enter message: [exam_type] [patient_name]");
            String message = br.readLine();

            // parse msg
            String[] msgArguments = message.split(" ");
            String destination_name = msgArguments[0];
            String destination_message = msgArguments[1];

            // break condition
            if ("exit".equals(message)) {
                break;
            }

            // publish
            channel.basicPublish(EXCHANGE_NAME, "info." + destination_name, null, destination_message.getBytes("UTF-8"));
            System.out.println("Sent: " + destination_message);
        }
    }
}
