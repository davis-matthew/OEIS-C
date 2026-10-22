/*
 * A390410  a(n) = Sum_{k=0..n} binomial(4*n+4*k,n-k).
 * https://oeis.org/A390410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   _binom_idx, _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} binomial(4*n + 4*k, n - k)
 *
 * The implementation computes each binomial coefficient by a multiplicative
 * method that avoids intermediate overflow.  It works for any seq_t
 * (8/16/32/64/128‑bit or arbitrary‑precision) and is valid C11 and C++17.
 */

#include <stdint.h>
#include "../utils/_binom_idx.h"
#include "../utils/_gcd_idx.h"

/* Greatest‑common‑divisor for idx_t values */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t N = 4 * n + 4 * k;         /* upper argument of binomial */
        idx_t K = n - k;                 /* lower argument          */
        sum += _binom_idx(N, K);
    }
    return sum;
}
