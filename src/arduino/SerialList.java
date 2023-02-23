package arduino;

import com.fazecast.jSerialComm.SerialPort;

import java.util.Objects;

/**
 * <p>Classe SerialList, utilizzata per gestire l'insieme delle classi {@link Serial}.</p>
 * Metodi principali:
 * <ul style="margin-top: 0px">
 *     <li>
 *         {@link #add(Serial)}
 *     </li>
 *     <li>
 *         {@link #remove(Serial)}
 *     </li>
 *     <li>
 *         {@link #autoInitialize()}
 *     </li>
 *     <li>
 *         {@link #close_all()}
 *     </li>
 *     <li>
 *         {@link #remove_all()}
 *     </li>
 *     <li>
 *         {@link #getSerials()}
 *     </li>
 *     <li>
 *         {@link #getSerialPortFromPort(String)}
 *     </li>
 * </ul>
 */
public class SerialList {

    /**
     * Array di oggetti {@link Serial}
     */
    private Serial[] serialPorts = new Serial[0];

    /**
     * Metodo utilizzato per aggiungere {@link DataCollector} all'array {@link #serialPorts}.
     *
     * @param serialPort {@link Serial}
     */

    public void add(Serial serialPort) {
        int noSerialports = this.serialPorts.length;
        Serial[] new_serialPorts = new Serial[noSerialports + 1];
        System.arraycopy(this.serialPorts, 0, new_serialPorts, 0, noSerialports);
        new_serialPorts[noSerialports] = serialPort;
        this.serialPorts = new_serialPorts;
    }

    /**
     * Metodo utilizzato per rimuovere {@link DataCollector} dall'array {@link #serialPorts}.
     *
     * @param serial {@link DataCollector}
     */
    public void remove(Serial serial) {
        Serial[] newSerialPorts = new Serial[this.serialPorts.length];
        int counter = 0;
        for (Serial current_serialport : this.serialPorts) {
            if (!Objects.equals(current_serialport.getPortLocation(), serial.getPortLocation())) {
                newSerialPorts[counter] = current_serialport;
                counter++;
            }
        }
        Serial[] new_collectors_resized = new Serial[counter];
        System.arraycopy(newSerialPorts, 0, new_collectors_resized, 0, counter);
        this.serialPorts = new_collectors_resized;
    }

    /**
     * Metodo utilizzato per auto-inizializzare solamente la prima porta seriale disponibile.
     * <p>
     * <b>
     * [SOLO PER TEST]
     * </b>
     * </p>
     */
    public void autoInitialize() {
        for (SerialPort port : SerialPort.getCommPorts()) {
            Serial serialPort = new Serial(port.getSystemPortName(), (port.getPortDescription().contains(" (COM")) ? port.getPortDescription().substring(0, port.getPortDescription().indexOf(" (COM")) : port.getPortDescription());
            if (serialPort.open()) {
                this.add(serialPort);
                break;
            }
        }
    }

    /**
     * Metodo utilizzato per chiudere la comunicazione con tutte le porte seriali registrate.
     */
    public void close_all() {
        for (Serial serial : this.serialPorts) {
            serial.close();
        }
    }

    /**
     * Metodo utilizzato per ripristinare l'array {@link #serialPorts}.
     */
    public void remove_all() {
        this.serialPorts = new Serial[0];
    }


    /**
     * Metodo utilizzato per ottenere la lista contenente i {@link Serial}.
     *
     * @return {@link #serialPorts} ({@code list})
     */
    public Serial[] getSerials() {
        return this.serialPorts;
    }

    /**
     * Metodo utilizzato per ottenere la seriale dal nome della porta.
     *
     * @param port Stringa contenente il nome della porta seriale
     * @return {@link Serial} <br>
     * <ul style="margin-top: 0px">
     *     <li>
     *         se {@link #serialPorts} contiene una porta seriale con tale nome
     *     </li>
     * </ul>
     * {@code null} <br>
     * <ul style="margin-top: 0px">
     *     <li>
     *         se non c'è stato nessun riscontro
     *     </li>
     * </ul>
     */
    public Serial getSerialPortFromPort(String port) {
        for (Serial serial : this.serialPorts) {
            if (Objects.equals(serial.getPortLocation(), port))
                return serial;
        }
        return null;
    }
}
