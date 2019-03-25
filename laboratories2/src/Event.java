public class Event {
    public enum State {
        PUT,
        REMOVE,
        GET,
        CONTAINS
    }
    private State state;
    private String key;
    private Integer value;

    public Event(State state, String key) {
        this.state = state;
        this.key = key;
    }

    public Event(State state, String key, Integer value) {
        this(state, key);
        this.value = value;
    }

    public Event(String[] arguments) throws Exception {
        checkArgumentsNumber(arguments);
        this.state = stringToState(arguments[0]);
        this.key = arguments[1];
        if(arguments.length == 3)
            this.value = Integer.valueOf(arguments[2]);
    }

    public Event(String arguments) throws Exception {
        String[] splitted = arguments.split("\\s+");
        checkArgumentsNumber(splitted);
        this.state = stringToState(splitted[0]);
        this.key = splitted[1];
        if(splitted.length == 3)
            this.value = Integer.valueOf(splitted[2]);
    }

    private State stringToState(String state) throws Exception {
        switch(state) {
            case "put" :        return State.PUT;
            case "remove" :     return State.REMOVE;
            case "get" :        return State.GET;
            case "contains" :   return State.CONTAINS;
            default : throw new Exception("Wrong argument");
        }
    }

    private String stateToString(State state) {
        switch(state) {
            case PUT :      return "put";
            case REMOVE :   return "remove";
            case GET :      return "get";
            case CONTAINS : return "contains";
        }
        return null;
    }

    private void checkArgumentsNumber(String[] arguments) throws Exception {
        if(arguments.length == 0)
            throw new Exception("Wrong amount of arguments");
        State state = stringToState(arguments[0]);
        if (state == State.PUT) {
            if (arguments.length != 3)
                throw new Exception("Wrong amount of arguments");
        } else {
            if (arguments.length != 2)
                throw new Exception("Wrong amount of arguments");
        }
    }

    public State getState() { return state; }
    public String getKey() { return key; }
    public Integer getValue() { return value; }

    public String toString() {
        String result = stateToString(state) + " " + key;
        if(isMethodWithTwoArguments())
            result = result + " " + value.toString();
        return result;
    }

    private boolean isMethodWithTwoArguments() {
        if(state == State.PUT)
            return true;
        return false;
    }
}

