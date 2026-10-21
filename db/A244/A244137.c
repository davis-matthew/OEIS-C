/*
 * A244137  Triangle read by rows: terms T(n,k) of a binomial decomposition of
 *          n^n as Sum(k=0..n)T(n,k).
 * https://oeis.org/A244137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)−1)/2⌋, k = n − r(r+1)/2; for k
 *            = 0 returns 0 (except n = 0 gives 1)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 66, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

/* Integer square‑root of a uint64_t using binary search. */
static idx_t int_isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* r = floor((sqrt(8n+1)-1)/2) */
    idx_t disc   = 8 * n + 1;
    idx_t sqrt_d = int_isqrt_u64(disc);
    idx_t r      = (sqrt_d - 1) / 2;

    /* k = n - r(r+1)/2 */
    idx_t triangular = r * (r + 1) / 2;
    idx_t k = n - triangular;

    if (k == 0) {
        /* n = 0 => T(0,0) = 1; n > 0 triangular => 0^n = 0 */
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* Compute T(r,k) = C(r,k) * k^(k-1) * (r-k)^(r-k) */
    seq_t term = binomial(r, k);

    if (k > 1) {
        term = term * pow_seq((seq_t)k, (int)(k - 1));
    }

    idx_t rmk = r - k;
    if (rmk > 0) {
        term = term * pow_seq((seq_t)rmk, (int)rmk);
    }
    /* when rmk == 0 the factor is 0^0 = 1, no need to multiply */

    return term;
}
