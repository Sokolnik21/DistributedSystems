import com.rabbitmq.client.*;

import java.io.IOException;

public class Technician {
    private String name;
    private String spec1;
    private String spec2;

    public String getName() { return this.name; }
    public String getSpec1() { return this.spec1; }
    public String getSpec2() { return this.spec2; }
    public String toString() { return this.name; }

    public static void main(String[] argv) throws Exception {
        // initialize
        Technician t = new Technician();
        t.name = argv[0];
        t.spec1 = argv[1];
        t.spec2 = argv[2];

        // info
        System.out.println(t.getName());

        // connection & channel
        Channel channel = ChannelProvider.configureChannel();
        channel.basicQos(1);

        // exchange
        String EXCHANGE_NAME = "Exchange";
        channel.exchangeDeclare(EXCHANGE_NAME, BuiltinExchangeType.TOPIC);

        // queue & bind
//        String queueName = channel.queueDeclare(t.getSpec1(), true, false, false, null).getQueue();
        String queueName = channel.queueDeclare().getQueue();
        channel.queueBind(queueName, EXCHANGE_NAME, "*.examination." + t.getSpec1());
        channel.queueBind(queueName, EXCHANGE_NAME, "*.examination." + t.getSpec2());
        System.out.println("created queue: " + queueName);

        // consumer (message handling)
        Consumer consumer = new DefaultConsumer(channel) {
            @Override
            public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body) throws IOException {
                String message = new String(body, "UTF-8");
                System.out.println("Received: " + message);

                // parse envelope
                String[] routing_key = envelope.getRoutingKey().split("\\.");
                String doctor_name = routing_key[0];
                String examination_type = routing_key[2];
                String patient_name = message;

                String new_message = patient_name + "," + examination_type+ "," + "done";

                // publish
                channel.basicPublish(EXCHANGE_NAME, t.getName() + ".result." + doctor_name, null, new_message.getBytes("UTF-8"));
                System.out.println("Sent: " + patient_name);

                channel.basicAck(envelope.getDeliveryTag(), false);
            }
        };

        // start listening
        System.out.println("Waiting for messages...");
        channel.basicConsume(queueName, false, consumer);
    }
}
