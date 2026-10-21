/*
 * A242954  a(n) = Product_{i=1..n} A234957(i).
 * https://oeis.org/A242954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(Σ_{k≥1} ⌊n / 4^k⌋)
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the product of A234957(i) for i=1..n, where
   A234957(i) = 4^{floor(v₂(i)/2)}, v₂(i) being the exponent of 2 in i. */

#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Compute S(n) = Σ_{k≥1} ⌊n / 4^k⌋, the total exponent of 4 in
       the product of 1..n.  The loop runs in O(log₄ n) steps. */
    idx_t S = 0;
    idx_t d = 4;
    while (d <= n) {
        S += n / d;
        /* Prevent overflow of d on the next multiplication. */
        if (d > n / 4) break;
        d *= 4;
    }
    /* a(n) = 4^S.  seq_pow handles arbitrary‑precision types. */
    return seq_pow((seq_t)4, S);
}
