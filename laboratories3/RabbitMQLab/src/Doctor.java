import com.rabbitmq.client.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Doctor {
    private String name;

    public String getName() { return this.name; }
    public String toString() { return this.name; }

    public static void main(String[] argv) throws Exception {
        // initialize
        Doctor d = new Doctor();
        d.name = argv[0];

        // info
        System.out.println(d.getName());

        // connection & channel
        Channel channel = ChannelProvider.configureChannel();

        // exchange
        String EXCHANGE_NAME = "Exchange";
        channel.exchangeDeclare(EXCHANGE_NAME, BuiltinExchangeType.TOPIC);

        // queue & bind
        String queueName = channel.queueDeclare().getQueue();
        channel.queueBind(queueName, EXCHANGE_NAME, "*.result." + d.getName());
        System.out.println("created queue: " + queueName);

        String finalQueueName = queueName;
        new Thread(new Runnable() {
            public void run()
            {
                // consumer (message handling)
                Consumer consumer = new DefaultConsumer(channel) {
                    @Override
                    public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body) throws IOException {
                        String message = new String(body, "UTF-8");
                        System.out.println("Received: " + message);

                        channel.basicAck(envelope.getDeliveryTag(), false);
                    }
                };

                // start listening
                System.out.println("Waiting for messages...");
                try {
                    channel.basicConsume(finalQueueName, false, consumer);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }}).start();

        queueName = channel.queueDeclare(d.getName(), true, false, false, null).getQueue();
        channel.queueBind(queueName, EXCHANGE_NAME, "info." + d.getName());
        System.out.println(d.getName());
        channel.basicConsume(queueName, false, new DefaultConsumer(channel) {
            @Override
            public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body) throws IOException {
                String message = new String(body, "UTF-8");
                System.out.println("Received ADMIN message: " + message);

                channel.basicAck(envelope.getDeliveryTag(), false);
            }
        });
        System.out.println("created queue: " + queueName);

        while (true) {

            // read msg
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            System.out.println("Enter message: [exam_type] [patient_name]");
            String message = br.readLine();

            // parse msg
            String[] msgArguments = message.split(" ");
            String exam_type = msgArguments[0];
            String patient_name = msgArguments[1];

            // break condition
            if ("exit".equals(message)) {
                break;
            }

            // publish
            channel.basicPublish(EXCHANGE_NAME, d.getName() + ".examination." + exam_type, null, patient_name.getBytes("UTF-8"));
            System.out.println("Sent: " + patient_name);
        }
    }
}
