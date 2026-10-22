/*
 * A390992  Sum of first sums of the binary indices of n.
 * https://oeis.org/A390992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of first sums of the binary indices of n
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binary indices of n are the positions (1‑based) of the 1‑bits in
   its reversed binary representation.  The algorithm computes the
   required sum by using the positions of set bits directly. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = 0;          /* number of set bits */
    idx_t minp = 0;       /* smallest set‑bit position (0‑based) */
    idx_t maxp = 0;       /* largest set‑bit position (0‑based) */
    seq_t sump = 0;       /* sum of all set‑bit positions */

    idx_t tmp = n;
    int first = 1;        /* flag for first set bit */

    while (tmp != 0) {
        /* remove lowest set bit and obtain its position */
        int pos = __builtin_ctzll(tmp);
        seq_t pos_s = (seq_t)pos;

        sump += pos_s;
        if (first) {
            minp = pos;
            first = 0;
        }
        maxp = pos;
        k++;

        tmp &= tmp - 1;   /* clear lowest set bit */
    }

    if (k <= 1)
        return 0;

    seq_t kk = (seq_t)k;
    seq_t result = 2 * sump - (seq_t)minp - (seq_t)maxp + 2 * (kk - 1);
    return result;
}
