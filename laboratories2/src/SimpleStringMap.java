public interface SimpleStringMap {
    void    put(String key, Integer value);
    Integer remove(String key);
    Integer get(String key);
    boolean containsKey(String key);
}
