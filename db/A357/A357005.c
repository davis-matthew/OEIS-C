/*
 * A357005  Smallest k that is cyclically equivalent (see Comment for
 *          definition) to n.
 * https://oeis.org/A357005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A357005 – smallest k cyclically equivalent to n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Computes the multiplicative GCD of two unsigned integers. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Returns the number of binary digits of a positive integer n. */
static inline idx_t bitlen_idx(idx_t n)
{
    /* n > 0 */
    return 64 - __builtin_clzll(n);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, but defensive */

    idx_t m = bitlen_idx(n);            /* bit‑length of n */
    seq_t min_k = (seq_t)n;             /* current best candidate */

    /* Positions of 1‑bits in n (0‑based from LSB). */
    idx_t pos[64];
    idx_t pop = 0;
    idx_t bits = n;
    while (bits) {
        idx_t trailing = __builtin_ctzll(bits);
        pos[pop++] = trailing;
        bits &= bits - 1;               /* clear lowest set bit */
    }

    /* Bit mask for the most significant position (= m‑1). */
    seq_t target = pow2(m - 1);

    /* Enumerate all affine maps with gcd(s,m)=1. */
    for (idx_t s = 1; s < m; ++s) {
        if (gcd_idx(s, m) != 1) continue;
        for (idx_t t = 0; t < m; ++t) {
            seq_t val = 0;
            for (idx_t i = 0; i < pop; ++i) {
                idx_t newpos = (s * pos[i] + t) % m;
                val |= pow2(newpos);
            }
            if ((val & target) == 0) continue;    /* highest bit must stay set */
            if (val < min_k) {
                min_k = val;
                if (min_k == (seq_t)1) return min_k;  /* cannot be smaller */
            }
        }
    }
    return min_k;
}
