/*
 * A279210  Length of second run of 1's in binary expansion of n.
 * https://oeis.org/A279210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Length of the second run of 1's in the binary expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute number of bits in n. */
    int bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }

    int current_run = 0;       /* 0: none, 1: first run, 2: second run, ... */
    bool in_run = false;       /* are we in a run of 1's?            */
    seq_t len_second = 0;      /* length of second run of 1's       */

    /* Scan bits from most significant to least. */
    for (int i = bitlen - 1; i >= 0; --i) {
        bool bit = (n >> i) & 1;
        if (bit) {
            if (!in_run) {                /* start of a new run */
                ++current_run;
                if (current_run == 2) {
                    len_second = 1;        /* first '1' of the second run */
                }
                in_run = true;
            } else {                       /* continuation of current run */
                if (current_run == 2) {
                    ++len_second;          /* count it */
                }
            }
        } else {
            in_run = false;                 /* run ended */
        }
    }
    return len_second;
}
