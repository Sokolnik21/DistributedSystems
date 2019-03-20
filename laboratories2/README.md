# laboratories2
Distributed hash map implementation via JGroups.

## specification
Application should implement SimpleStringMap interface(*) <br>
Based on CAP theorem, implementation should provide 2 out of 3 guarantees:
- Availability: Every request receives a (non-error) response – without the guarantee that it contains the most recent write
- Partition tolerance: The system continues to operate despite an arbitrary number of messages being dropped (or delayed) by the network between nodes

In case of partition, every subgroup should work independently. <br>
After megre one group loses it's state and aquires new from the other one.

## SimpleStringMap interface
```java
public interface SimpleStringMap {
	void    put(String key, Integer value);
	Integer remove(String key);
	Integer get(String key);
	boolean containsKey(String key);
}
```
