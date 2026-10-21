/*
 * A270570  Largest number in the sequence for the Collatz problem (excluding
 *          the original number) when started at n.
 * https://oeis.org/A270570
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum value of the Collatz sequence that starts at n,
 *            excluding the starting number itself
 * Range:     u8 n <= 26, u16 n <= 702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t current = (seq_t)n;
    seq_t max_val  = 0;

    while (1) {
        seq_t next;
        if (current & 1) {
            next = current * 3 + 1;   /* 3n + 1 for odd */
        } else {
            next = current >> 1;      /* n / 2 for even */
        }

        if (next > max_val) {
            max_val = next;
        }

        if (next == 1) {
            break;
        }

        current = next;
    }

    return max_val;
}
