/*
 * A261115  Expansion of f(x, x) * f(x^4, x^8) in powers of x where f(,) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A261115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(i,j) ∈ ℤ² : i² + 2·j(3j‑1) = n} (OEIS A261115)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root, floor */
static idx_t _isqrt(idx_t n)
{
    idx_t r = 0;
    /* avoid overflow: (r+1)² ≤ n  ⇔ (r+1) ≤ n/(r+1) */
    while ((r + 1) <= n / (r + 1)) {
        ++r;
    }
    return r;
}

/* Count representations */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* √n provides the maximum |i| that can occur */
    idx_t max_i = _isqrt(n);

    for (long long i = -(long long)max_i; i <= (long long)max_i; ++i) {
        /* i² is always non‑negative; compute as signed to avoid overflow */
        idx_t i2 = (idx_t)((long long)i * (long long)i);
        if (i2 > n) continue;             /* safety guard             */
        idx_t rem = n - i2;                /* remaining to be written by j */
        if (rem & 1) continue;             /* rem must be even          */

        /* Determine the range of j that could satisfy 2j(3j-1)=rem */
        idx_t j_lim = 0;
        for (long long j = 0;; ++j) {
            idx_t val = (idx_t)(2LL * j * (3LL * j - 1));
            if (val > rem) { j_lim = j - 1; break; }
        }

        for (long long j = -(long long)j_lim; j <= (long long)j_lim; ++j) {
            idx_t val = (idx_t)(2LL * j * (3LL * j - 1));
            if (val == rem) ++count;
        }
    }

    return count;
}
