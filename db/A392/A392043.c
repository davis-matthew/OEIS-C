/*
 * A392043  a(n) = Sum_{k=0..floor(n/3)} (k+1) * 2^k * 3^(n-3*k) *
 *          binomial(k,3*(n-3*k)).
 * https://oeis.org/A392043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} (k+1) * 2^k * 3^(n-3k) *
 *            binomial(k,3*(n-3k))
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 43, u64 n <= 87, u128 n <= 174,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* simple summation; takes O(n) time with at most ⌊n/3⌋ iterations */
    idx_t limit = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= limit; ++k)
    {
        idx_t m  = n - 3 * k;          /* remaining exponent for 3 */
        idx_t r  = 3 * m;              /* argument to binomial */
        if (r > k)                     /* binomial(k,r) == 0 otherwise */
            continue;

        seq_t term = (seq_t)(k + 1);
        term *= pow2(k);
        term *= pow3(m);
        term *= binomial(k, r);
        sum += term;
    }
    return sum;
}
