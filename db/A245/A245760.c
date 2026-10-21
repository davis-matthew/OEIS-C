/*
 * A245760  Maximal multiplicative persistence of n in any base.
 * https://oeis.org/A245760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   persistence_in_base, product_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximal multiplicative persistence of n in any base 2 … n */
#include <stdbool.h>
#include "../utils/persistence_in_base.h"
#include "../utils/product_digits.h"

/* Sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 0 by definition */
        return 0;

    seq_t max_persistence = 0;
    seq_t n_val = (seq_t)n;

    for (idx_t b = 2; b <= n; ++b) {
        seq_t p = persistence_in_base(n_val, (seq_t)b);
        if (p > max_persistence)
            max_persistence = p;
    }
    return max_persistence;
}
