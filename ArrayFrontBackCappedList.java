
public class ArrayFrontBackCappedList<T> implements FrontBackCappedList<T> {

    private T[] list;
    private int numberOfElements;

    ArrayFrontBackCappedList(int capacity) {
        list = (T[]) new Object[capacity];
    }

    @Override
    public boolean addFront(T newEntry) {
        if (!isFull()) {
            for (int i = size(); i > 0; i--) {
                this.list[i] = this.list[i - 1];
            }
            this.list[0] = newEntry;
            this.numberOfElements++;
            return true;
        }
        return false;
    }

    @Override
    public boolean addBack(T newEntry) {
        if (!isFull()) {
            this.list[size()] = newEntry;
            numberOfElements++;
            return true;
        }
        return false;
    }

    @Override
    public T removeFront() {
        if (isEmpty()) {
            return null;
        }
        T removedItem = this.list[0];
        for (int i = 0; i < size() - 1; i++) {
            this.list[i] = this.list[i + 1];
        }
        numberOfElements--;
        return removedItem;
    }

    @Override
    public T removeBack() {
        if (!isEmpty()) {
            T removedItem = this.list[size() - 1];
            this.list[size() - 1] = null;
            numberOfElements--;
            return removedItem;
        }
        return null;
    }

    @Override
    public void clear() {
        for (int i = 0; i < size(); i++) {
            this.list[i] = null;
        }
        this.numberOfElements = 0;
    }

    @Override
    public T getEntry(int givenPosition) {
        if (givenPosition >= 0 && givenPosition < size()) {
            return this.list[givenPosition];
        }
        return null;
    }

    @Override
    public int indexOf(T anEntry) {
        for (int i = 0; i < size(); i++) {
            if (list[i].equals(anEntry)) {
                return i;
            }
        }
        return -1;
    }

    @Override
    public int lastIndexOf(T anEntry) {
        for (int i = size() - 1; i >=0; i--) {
            if (list[i].equals(anEntry)) {
                return i;
            }
        }
        return -1;
    }

    @Override
    public boolean contains(T anEntry) {
        for (int i = 0; i < size(); i++) {
            if (list[i].equals(anEntry)) {
                return true;
            }
        }
        return false;
    }

    @Override
    public int size() {
        return this.numberOfElements;
    }

    @Override
    public boolean isEmpty() {
        return this.size() == 0;
    }

    @Override
    public boolean isFull() {
        return this.size() == this.list.length;
    }

    @Override
    public String toString() {
        StringBuilder s = new StringBuilder("[");
        for(int i=0; i<size(); i++) {
            s.append(list[i]).append(", ");
        }
        if(!isEmpty()) {
            s = new StringBuilder(s.substring(0, s.length() - 2));
        }
        s.append("]");
        return "size="  + size() + "; capacity=" + this.list.length + ";  " + s;
    }
}
