/*
 * A327320  Triangular array read by rows: row n shows the coefficients of the
 *          polynomial p(x,n) constructed as in Comments; these polynomials
 *          form a strong divisibility sequence.
 * https://oeis.org/A327320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   let r be the unique integer with r*(r+1)/2 ≥ n, let k = n -
 *            r*(r-1)/2 - 1. Then a(n) = binomial(r,k) * (2^r - (-1)^{r+k}
 *            2^k) / 3^{v3(r)+1}, where v3(r) is the exponent of 3 in r
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

/* Helper: exponent of 3 in r. */
static idx_t v3(idx_t r)
{
    idx_t e = 0;
    while (r % 3 == 0) {
        ++e;
        r /= 3;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined; return 0 for safety. */

    /* Find r such that r*(r+1)/2 >= n.  r is at most 200 for n <= 10000. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column index k. */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev - 1;

    /* Cache powers of 2 up to r (small array). */
    static seq_t pow2_cache[200];
    static unsigned char pow2_valid[200] = {0};

    if (!pow2_valid[r]) {
        pow2_cache[r] = pow2(r);
        pow2_valid[r] = 1;
    }
    if (!pow2_valid[k]) {
        pow2_cache[k] = pow2(k);
        pow2_valid[k] = 1;
    }

    seq_t pow2_r = pow2_cache[r];
    seq_t pow2_k = pow2_cache[k];

    /* diff = 2^r - (-1)^{r+k} * 2^k  == 2^r - 2^k  if (r+k) even,
       otherwise 2^r + 2^k. */
    seq_t diff;
    if (((r + k) & 1u) == 0) {
        diff = pow2_r - pow2_k;
    } else {
        diff = pow2_r + pow2_k;
    }

    seq_t bin = binomial(r, k);
    seq_t coeff = bin * diff;

    /* g = 3^{v3(r)+1}. */
    idx_t exp = v3(r) + 1;
    seq_t g = pow3(exp);

    return coeff / g;
}
