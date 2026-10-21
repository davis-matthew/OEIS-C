/*
 * A292526  Expansion of x^1 * (1-x^1) / (1+x^1)^2 - x^4 * (1-x^3) * (1+x^3)^2
 *          + x^9 * (1-x^5) / (1+x^5)^2 - ... in powers of x.
 * https://oeis.org/A292526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1}^{⌊√n⌋} -( -1)^k * ( (-1)^j ) * (2j+1) , where * j
 *            = (n - k²)/(2k-1) is an integer (otherwise the summand is 0).
 *            This * follows from expanding the generating function * Σ_{k>0}
 *            -(-1)^k x^(k²)(1-x^(2k-1))/(1+x^(2k-1))²
 * Range:     i8 n <= 48, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u64(idx_t n)
{
    /* Integer square root: floor(sqrt(n)) for unsigned 64-bit n. */
    idx_t r = 0, hi = n;
    while (r + 1 <= hi) {
        idx_t mid = (r + hi + 1) >> 1;
        if (mid <= n / mid)
            r = mid;
        else
            hi = mid - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = isqrt_u64(n);
    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t base = k * k;
        if (base > n) break;
        idx_t mprime = n - base;
        idx_t d = 2 * k - 1;          /* d = 2k-1 */
        if (mprime % d != 0) continue;
        idx_t j = mprime / d;         /* j is guaranteed integer */

        /* coefficient from the inner series: (-1)^j * (2j+1) */
        seq_t coeff = ((j & 1) ? -1 : 1) * ((seq_t)2 * (seq_t)j + 1);

        /* outer factor: -( -1)^k = (k odd ? 1 : -1) */
        seq_t sign_k = (k & 1) ? 1 : -1;

        seq_t contrib = sign_k * coeff;
        result += contrib;
    }
    return result;
}
