/*
 * A400604  a(1) = 1; a(n+1) = Sum_{d|n} Sum_{j|d} a(d) * a(n/d) * a(d/j).
 * https://oeis.org/A400604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(1) = 1; a(n+1) = Σ_{d|n} Σ_{j|d} a(d) * a(n/d) * a(d/j) for n
 *            ≥ 1 (OEIS sequence A400604)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declarations of types used by the build system */
typedef uint64_t idx_t;   /* index type */
/* seq_t is defined by the build system (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128,
   or an arbitrary‑precision integer class). */

static seq_t *a_cache = NULL;     /* cache of a(n) values */
static size_t a_cap = 0;          /* allocated capacity of a_cache (≥ index + 1) */
static idx_t last_computed = 0;   /* largest index for which a_cache[i] is valid */

/* Ensure that cache arrays have capacity at least 'needed + 1' */
static void
ensure_cap(idx_t needed, seq_t **arr, size_t *cap)
{
    if (needed + 1 <= *cap)
        return;
    size_t new_cap = (needed + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);               /* allocation failure */
    *arr = tmp;
    *cap = new_cap;
}

/* Integer floor square root (binary search, safe for idx_t = uint64_t) */
static inline idx_t
floor_isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t lo = 0, hi = n / 2 + 1;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid <= n / mid) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Compute a(n) with memoization */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined behavior for n < 1 */

    /* Expand cache to hold index n */
    ensure_cap(n, &a_cache, &a_cap);

    /* Initialize the first term if necessary */
    if (last_computed == 0) {
        a_cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* Compute missing terms up to n */
    while (last_computed < n) {
        idx_t m = last_computed;           /* m = n-1 for this iteration */
        seq_t sum = (seq_t)0;

        idx_t sqrt_m = floor_isqrt_u64(m);
        for (idx_t d = 1; d <= sqrt_m; ++d) {
            if (m % d != 0)
                continue;

            idx_t d1 = d;
            idx_t d2 = m / d;

            /* Inner divisor sum for d1: Σ_{k|d1} a(k) */
            seq_t inner1 = (seq_t)0;
            idx_t sqrt_d1 = floor_isqrt_u64(d1);
            for (idx_t k = 1; k <= sqrt_d1; ++k) {
                if (d1 % k != 0)
                    continue;
                inner1 += a_cache[k];
                if (k != d1 / k)
                    inner1 += a_cache[d1 / k];
            }
            /* Term for pair (d1, d2) */
            seq_t term1 = a_cache[d1] * a_cache[d2] * inner1;
            sum += term1;

            if (d1 != d2) {
                /* Inner divisor sum for d2: Σ_{k|d2} a(k) */
                seq_t inner2 = (seq_t)0;
                idx_t sqrt_d2 = floor_isqrt_u64(d2);
                for (idx_t k = 1; k <= sqrt_d2; ++k) {
                    if (d2 % k != 0)
                        continue;
                    inner2 += a_cache[k];
                    if (k != d2 / k)
                        inner2 += a_cache[d2 / k];
                }
                /* Term for pair (d2, d1) */
                seq_t term2 = a_cache[d2] * a_cache[d1] * inner2;
                sum += term2;
            }
        }

        a_cache[last_computed + 1] = sum;
        ++last_computed;
    }

    return a_cache[n];
}
