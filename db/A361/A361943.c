/*
 * A361943  a(n) is the least multiple of n whose binary expansion is an
 *          abelian square (A272653).
 * https://oeis.org/A361943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least multiple of n whose binary expansion is an abelian
 *            square (A272653)
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Compute number of bits needed to represent x (used for any seq_t type). */
static inline size_t bitlen_seq_t_local(seq_t x) {
    size_t len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;          /* cast index to seq_t once */
    for (idx_t f = 1;; ++f) {    /* search for the minimal multiplier f */
        seq_t m = N * (seq_t)f;  /* current multiple of n */
        size_t len = bitlen_seq_t_local(m);
        if ((len & 1) == 0) {    /* only even‐length numbers can be abelian squares */
            size_t half = len >> 1;
            size_t ones1 = 0, ones2 = 0;
            for (size_t i = 0; i < half; ++i) {
                if ((m >> i) & 1) ++ones1;
                if ((m >> (i + half)) & 1) ++ones2;
            }
            if (ones1 == ones2)    /* halves have the same number of ones (hence zeros) */
                return m;
        }
    }
}
