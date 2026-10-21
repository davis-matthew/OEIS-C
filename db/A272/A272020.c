/*
 * A272020  Irregular triangle read by rows: strictly decreasing sequences of
 *          positive numbers given in lexicographic order.
 * https://oeis.org/A272020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) enumerates the exponents of 2 in the binary expansion of
 *            2*k, reading the decreasing sequences in lexicographic order.
 *            The implementation generates rows k≥1, extracts the set‑bit
 *            positions of 2*k from most significant to least significant, and
 *            returns the n‑th element of this stream
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Global position in the concatenated list of exponents */
    idx_t global = 0;

    for (idx_t k = 1; ; ++k) {             /* start from row 1 (row 0 is empty) */
        unsigned long long v = (unsigned long long)k << 1;   /* 2*k        */
        while (v != 0) {
            int pos;
            for (pos = 63; pos >= 0; --pos) {
                if (v & (1ULL << pos))
                    break;
            }
            if (global == n)
                return (seq_t)pos;
            ++global;
            v &= ~(1ULL << pos);            /* clear the most‑significant set bit */
        }
    }
}
