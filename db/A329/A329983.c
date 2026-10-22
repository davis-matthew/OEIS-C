/*
 * A329983  For any n > 0, define the sequence b(1) = n, b(i+1) = (b(i) * i)
 *          mod (b(i) + i); a(n) is the least i such that b(i) = 0, or -1 if 0
 *          is never reached.
 * https://oeis.org/A329983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least i ≥ 1 such that b(1) = n, b(i+1) = (b(i) * i) mod
 *            (b(i) + i); a(n) = i, or (seq_t)-1 if no such i exists
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case: n == 0 → b(1) = 0, so a(0) = 1 */
    if (n == 0) return (seq_t)1;

    seq_t b = (seq_t)n;          /* b(1) */
    for (idx_t i = 1; ; ++i) {
        if (b == 0) return (seq_t)i;
        seq_t mod = b + (seq_t)i;                      /* b(i)+i  */
        /* Compute (b * i) % mod using a wider type to avoid overflow
           of 64‑bit seq_t.  For seq_t larger than 64 bits the
           multiplication is handled by the arbitrary‑precision
           library.  */
        seq_t numerator = (seq_t)(((__uint128_t)b * i) % mod);
        b = numerator;
    }
}
