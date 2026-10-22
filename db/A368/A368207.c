/*
 * A368207  Bacher numbers: number of nonnegative representations of n =
 *          w*x+y*z with max(w,x) < min(y,z).
 * https://oeis.org/A368207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A368207: Bacher numbers
   Formula: a(n) = Σ_{d∈t(n)} s(d,n) + Σ_{k=1..⌊n/2⌋} Σ_{w∈t(k)} Σ_{y∈t(n−k)} [k<y·w]·max(1,2·([w²<k]+[y²<n−k]))
   where t(n) = {d | d·d ≤ n, d divides n}
   and s(d,n) = 2·d−1 if d·d = n else 4·d−2 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer floor square‑root for unsigned 64‑bit values. */
static inline uint64_t floor_isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)  /* mid*mid > n avoiding overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1, safe fallback */

    seq_t sum = 0;
    uint64_t N = (uint64_t)n;

    /* First sum over small divisors d ≤ √N */
    uint64_t limit = floor_isqrt_u64(N);
    for (uint64_t d = 1; d <= limit; ++d)
    {
        if (N % d != 0) continue;
        if (d * d == N)
            sum += (seq_t)(2 * d - 1);
        else
            sum += (seq_t)(4 * d - 2);
    }

    /* Second double sum over k, w, y */
    for (uint64_t k = 1; k <= N / 2; ++k)
    {
        uint64_t NK   = N - k;
        uint64_t wlim = floor_isqrt_u64(k);
        uint64_t ylim = floor_isqrt_u64(NK);

        for (uint64_t w = 1; w <= wlim; ++w)
        {
            if (k % w != 0) continue;

            for (uint64_t y = 1; y <= ylim; ++y)
            {
                if (NK % y != 0) continue;

                /* Condition k < y*w (watch for overflow) */
                if (k >= y * w) continue;

                /* Flags for squares */
                unsigned flags = 0;
                if (w * w < k) ++flags;
                if (y * y < NK) ++flags;

                /* Contribution = max(1, 2*flags) */
                seq_t contrib = (flags == 0) ? 1 : (seq_t)(2 * flags);
                sum += contrib;
            }
        }
    }

    return sum;
}
