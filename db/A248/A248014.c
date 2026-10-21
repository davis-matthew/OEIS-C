/*
 * A248014  Numbers m such that A247796(m) < m.
 * https://oeis.org/A248014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th integer m ≥ 10 that contains at least one pair of adjacent
   decimal digits whose sum ≤ 9. This is the definition of OEIS A248014. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t m = 10;                     /* first candidate */
    idx_t count = 0;                  /* how many good numbers found */

    while (1) {
        seq_t x = m;
        bool has_pair = false;        /* flag for a qualifying adjacent pair */

        if (x >= (seq_t)10) {         /* need at least two digits */
            while (x >= (seq_t)10) {
                seq_t d2 = x % (seq_t)10;    /* least‑significant digit */
                x = x / (seq_t)10;           /* drop the digit */
                seq_t d1 = x % (seq_t)10;    /* next digit towards left */
                if (d1 + d2 <= (seq_t)9) {
                    has_pair = true;
                    break;                 /* no need to check further */
                }
            }
        }

        if (has_pair) {
            count++;                   /* valid number found */
            if (count == n)            /* wanted n‑th value */
                return m;
        }

        m++;                           /* next integer to test */
    }
}
