/*
 * A387483  a(n) = Sum_{k=0..floor(n/2)} 2^(n-k) * binomial(k,n-2*k)^2.
 * https://oeis.org/A387483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 2^(n-k) * binomial(k,n-2*k)^2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 2 * k;
        if (m > k)          /* binomial(k,m) would be 0 for m > k */
            continue;

        seq_t bin = binomial(k, m);          /* binomial(k, n-2k) */
        seq_t pw  = pow2(n - k);              /* 2^(n-k) */

        /* term = pw * bin * bin ; multiplication commutes for
           integer and arbitrary‑precision types. Overflows wrap
           for fixed‑width types, as required. */
        seq_t term = pw * bin * bin;

        sum += term;
    }

    return sum;
}
