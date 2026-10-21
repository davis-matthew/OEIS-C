/*
 * A269695  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 6", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^popcount(floor(n/2)) * (n%2 ? 5 : 1)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* count set bits in an unsigned 64‑bit integer */
static inline unsigned popcount_u64(idx_t x) {
    /* standard Kernighan algorithm */
    unsigned count = 0;
    while (x) {
        x &= (x - 1);
        ++count;
    }
    return count;
}

seq_t a(idx_t n) {
    idx_t m = n >> 1;                  /* floor(n/2) */
    unsigned k = popcount_u64(m);      /* exponent of 4 */

    seq_t result = big_pow((seq_t)4, (seq_t)k);   /* 4^k */

    if (n & 1)                         /* odd n -> multiply by 5 */
        result *= (seq_t)5;

    return result;
}
