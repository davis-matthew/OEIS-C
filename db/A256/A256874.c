/*
 * A256874  Numbers divisible by prime(d) for each digit d in their base-4
 *          representation, none of which may be zero.
 * https://oeis.org/A256874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers divisible by prime(d) for each digit d in their base‑4 *
 *            representation, none of which may be zero. The primes used are *
 *            prime(1)=2, prime(2)=3, prime(3)=5. The sequence is computed by
 *            * brute‐force search of the integers in ascending order and
 *            selecting * those satisfying the digit and...
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined, but guard against zero */

    seq_t current = 1;
    idx_t count = 0;

    for (;;) {
        seq_t temp = current;
        bool zero_digit = false;
        bool need2 = false, need3 = false, need5 = false;

        /* extract base‑4 digits */
        while (temp > 0) {
            seq_t dseq = temp % 4;
            unsigned d = (unsigned)dseq;

            if (d == 0) {
                zero_digit = true;
                break;
            }
            if (d == 1) need2 = true;
            else if (d == 2) need3 = true;
            else if (d == 3) need5 = true;

            temp = temp / 4;
        }

        if (!zero_digit) {
            bool ok = true;
            if (need2 && (current % 2) != 0) ok = false;
            if (need3 && (current % 3) != 0) ok = false;
            if (need5 && (current % 5) != 0) ok = false;

            if (ok) {
                ++count;
                if (count == n) return current;
            }
        }

        ++current;
    }
}
