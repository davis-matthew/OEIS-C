/*
 * A267861  Number of ways to write n as 2*t + u^4 + v^4 + 2*w^4 + 3*x^4 +
 *          4*y^4 + 6*z^4, where t is 0 or 1, and u, v, w, x, y, z are
 *          nonnegative integers with u <= v and v > 0.
 * https://oeis.org/A267861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of solutions to * 2*t + u^4 + v^4 + 2*w^4 + 3*x^4
 *            + 4*y^4 + 6*z^4 = n, * with t∈{0,1}, u,v,w,x,y,z∈ℕ, u≤v, v>0. *
 *            The routine pre‑computes all possible values of * 2*w^4 + 3*x^4
 *            + 4*y^4 + 6*z^4 (w,x,y,z ≥ 0) * up to the largest requested n
 *            and then enumerates all admissible (u,v,t)
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fourth_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/fourth_root.h"

/* ----- cache for sums of the form 2*w^4 + 3*x^4 + 4*y^4 + 6*z^4 -------- */
static seq_t *cnt4 = NULL;      /* local cached counts */
static idx_t  precomp_limit = 0;/* largest N for which cnt4 is valid */

/* allocate and initialise buffer of size limit+1, recompute all sums */
static void precompute_cnt4(idx_t limit)
{
    /* Release old buffer if it exists */
#ifdef __cplusplus
    delete[] cnt4;
#else
    free(cnt4);
#endif
    /* Allocate new buffer */
#ifdef __cplusplus
    seq_t *new_arr = new seq_t[limit + 1];
#else
    seq_t *new_arr = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
#endif

    /* initialise to zero */
    for (idx_t i = 0; i <= limit; ++i) new_arr[i] = 0;

    /* enumerate all w,x,y,z producing values ≤ limit */
    for (idx_t w = 0; ; ++w)
    {
        seq_t w4   = (seq_t)w * w * w * w;
        seq_t valw = w4 * 2;
        if (valw > limit) break;

        for (idx_t x = 0; ; ++x)
        {
            seq_t x4   = (seq_t)x * x * x * x;
            seq_t valx = x4 * 3;
            if (valx > limit) break;

            for (idx_t y = 0; ; ++y)
            {
                seq_t y4   = (seq_t)y * y * y * y;
                seq_t valy = y4 * 4;
                if (valy > limit) break;

                for (idx_t z = 0; ; ++z)
                {
                    seq_t z4   = (seq_t)z * z * z * z;
                    seq_t valz = z4 * 6;
                    seq_t sum  = valw + valx + valy + valz;
                    if (sum > limit) break;
                    ++new_arr[(idx_t)sum];
                }
            }
        }
    }
    cnt4 = new_arr;
    precomp_limit = limit;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* outside the offset */

    /* make sure the cache covers this n */
    if (precomp_limit < n) precompute_cnt4(n);

    seq_t result = 0;

    idx_t vmax = fourth_root(n);             /* v > 0 */
    for (idx_t v = 1; v <= vmax; ++v)
    {
        seq_t v4 = (seq_t)v * v * v * v;
        for (idx_t u = 0; u <= v; ++u)       /* u ≤ v, u ≥ 0 */
        {
            seq_t u4   = (seq_t)u * u * u * u;
            seq_t base = u4 + v4;
            if (base > n) continue;          /* remaining would be negative */

            for (idx_t t = 0; t <= 1; ++t)   /* t ∈ {0,1} */
            {
                seq_t total = base + (seq_t)t * 2;
                if (total > n) continue;
                seq_t remaining = (seq_t)n - total;
                result += cnt4[(idx_t)remaining];
            }
        }
    }
    return result;
}
