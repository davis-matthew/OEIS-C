/*
 * A337392  Minimum m such that the convergence speed of m^^m is equal to n >=
 *          2, where A317905(n) represents the convergence speed of m^^m (and
 *          m = A067251(n), the n-th non-multiple of 10).
 * https://oeis.org/A337392
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (2 - (-1)^n) * 2^n + i^((n+1)*(n+2)) (here i^k is +1 if k
 *            ≡ 0 (mod 4), and –1 if k ≡ 2 (mod 4))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined for indices below the offset */

    /* 2^n as a seq_t */
    seq_t pow = pow2(n);

    /* (2 - (-1)^n) * 2^n  ->  2^n for even n, 3*2^n for odd n */
    seq_t term1 = (n & 1) ? mul_small(pow, 3) : pow;

    /* i^((n+1)*(n+2)) is +1 when (n+1)*(n+2) ≡ 0 (mod 4),
       otherwise –1 (since the exponent is always even) */
    idx_t prod_mod4 = ((n + 1) * (n + 2)) & 3; /* % 4 */

    if (prod_mod4 == 0)
        return term1 + 1;
    else
        return term1 - 1;
}
