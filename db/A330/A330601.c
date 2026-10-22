/*
 * A330601  Array T read by antidiagonals: T(m,n) is the number of lattice
 *          walks from (0,0) to (m,n) using one step from {(3,0), (2,1),
 *          (1,2), (0,3)} and all other steps from {(1,0), (0,1)}.
 * https://oeis.org/A330601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) is the antidiagonal read of the array T(m,n) defined by
 *            T(m,n) = (m+n-2)* (binomial(m+n-2,m) + binomial(m+n-2,n))
 * Range:     u8 n <= 47, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation converts the linear index n into diagonal coordinates (m,n),
   then evaluates the closed‑form formula using the provided binomial routine. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* Integer square‑root of an idx_t value (floor).  Uses 128‑bit arithmetic
   to avoid overflow. */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL;            /* sqrt(2^64‑1) */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find the diagonal s such that (s+1)(s+2)/2 > n.
       The approximate formula gives s = floor((sqrt(8n+1)-1)/2). */
    idx_t sqrt_val = floor_isqrt_u64(8 * n + 1);
    idx_t s = (sqrt_val - 1) / 2;

    /* Ensure that we have the smallest s satisfying the inequality. */
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* Coordinate within diagonal s */
    idx_t diag_start = s * (s + 1) / 2;
    idx_t offset = n - diag_start;
    idx_t m_val = offset;          /* 0 <= m <= s */
    idx_t n_val = s - offset;      /* n = s - m */

    if (s < 2)                     /* T(m,n)=0 for m+n<2 */
        return 0;

    /* Compute T(m,n) = (s-2)*(C(s-2,m)+C(s-2,n)) */
    idx_t n_minus2 = s - 2;
    seq_t bin1 = 0, bin2 = 0;

    if (m_val <= n_minus2)
        bin1 = binomial(n_minus2, m_val);
    if (n_val <= n_minus2)
        bin2 = binomial(n_minus2, n_val);

    seq_t sum    = bin1 + bin2;
    seq_t factor = (seq_t)(s - 2);
    return factor * sum;
}
