/*
 * A392075  a(n) = Sum_{k=0..floor(n/3)} (k+1) * 2^k * 3^(n-3*k) *
 *          binomial(k,4*(n-3*k)).
 * https://oeis.org/A392075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)}
 *            (k+1)*2^k*3^(n-3*k)*binomial(k,4*(n-3*k))
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 43, u64 n <= 87, u128 n <= 178,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute limits for k: only k with 4*(n-3k) ≤ k contribute */
    idx_t max_k = n / 3;
    idx_t min_k = (4 * n + 12) / 13;   /* ceil(4*n/13) */

    if (min_k > max_k)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    for (idx_t k = min_k; ; ++k)
    {
        idx_t t = n - 3 * k;          /* exponent of 3, always ≥ 0 */
        idx_t m = 4 * t;              /* second argument of binomial */

        /* binomial(k,4*(n-3k)) is zero for m>k, but our min_k guarantees it is ≤ k */
        seq_t bin = binomial(k, m);

        seq_t term = (seq_t)(k + 1);   /* (k+1) */
        term *= pow2(k);               /* 2^k */
        term *= pow3(t);               /* 3^(n-3k) */
        term *= bin;                   /* * binomial(k,4*(n-3k)) */

        sum += term;

        if (k == max_k)
            break;
    }

    return sum;
}
