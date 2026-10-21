/*
 * A254734  a(n) is the least k > n such that n divides k^4.
 * https://oeis.org/A254734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the OEIS sequence A254734. */
/* For a given n ≥ 1, it returns the smallest k > n such that n divides k^4. */
/* By the OEIS comment, this k is always ≤ 2 n, so a linear search over (n, 2n] is sufficient. */

#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)n;            /* modulus n (as seq_t) */
    idx_t start = n + 1;             /* first candidate k > n */
    idx_t end   = n * 2;             /* guaranteed upper bound */

    for (idx_t k = start; k <= end; ++k) {
        /* Check if k^4 ≡ 0 (mod n).  mod_pow handles modular multiplication safely. */
        if (mod_pow((seq_t)k, 4, mod) == 0)
            return (seq_t)k;        /* found the least such k */
    }

    /* According to the sequence definition, this point is never reached. */
    return 0;
}
