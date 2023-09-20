import java.util.ArrayList;
import java.util.List;

public class ListFrontBackCappedList<T> implements FrontBackCappedList<T> {


    private final List<T> list;

    private int capacity;
    private int size;

    public ListFrontBackCappedList(int capacity) {
        this.list = new ArrayList<>();
        this.capacity = capacity;
        this.size = 0;
    }

    @Override
    public boolean addFront(T newEntry) {
        if (!isFull()) {
            this.list.add(0,newEntry);
            size++;
            return true;
        }
        return false;
    }

    @Override
    public boolean addBack(T newEntry) {
        if (!isFull()) {
            this.list.add(size(),newEntry);
            size++;
            return true;
        }
        return false;
    }

    @Override
    public T removeFront() {
        if (isEmpty()) {
            return null;
        }
        T removedItem = this.list.remove(0);
        size--;
        return removedItem;
    }

    @Override
    public T removeBack() {
        if (isEmpty()) {
            return null;
        }
        T removedItem = this.list.remove(size() - 1);
        size--;
        return  removedItem;
    }

    @Override
    public void clear() {
        this.size = 0;
        this.list.clear();
    }

    @Override
    public T getEntry(int givenPosition) {
        if (givenPosition >= 0 && givenPosition < size) {
            return this.list.get(givenPosition);
        }
        return null;
    }

    @Override
    public int indexOf(T anEntry) {
        return this.list.indexOf(anEntry);
    }

    @Override
    public int lastIndexOf(T anEntry) {
        return this.list.lastIndexOf(anEntry);
    }

    @Override
    public boolean contains(T anEntry) {
        return this.list.contains(anEntry);
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    @Override
    public boolean isFull() {
        return size == capacity;
    }

    @Override
    public String toString() {
        StringBuilder s = new StringBuilder("[");
        for(int i=0; i<size(); i++) {
            s.append(list.get(i)).append(", ");
        }
        if(!isEmpty()) {
            s = new StringBuilder(s.substring(0, s.length() - 2));
        }
        s.append("]");
        return "size="  + size() + "; capacity=" + this.capacity + ";  " + s;
    }
}
