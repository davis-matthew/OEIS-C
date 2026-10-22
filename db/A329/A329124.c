/*
 * A329124  Dirichlet g.f.: Product_{k>=2} 1 / (1 - k^(-s))^(k*(k + 1)/2).
 * https://oeis.org/A329124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A329124
 *  Dirichlet generating function: ∏_{k≥2}(1‑k^(‑s))^(‑k(k+1)/2)
 *
 *  a(n) = coefficient of 1/n^s in the above product.
 *  Computed by iteratively multiplying the Dirichlet series for each k.
 *
 *  Algorithm:
 *    a[1] = 1
 *    For k = 2 … n
 *        c   = k(k+1)/2
 *        for each exponent e ≥ 1 such that k^e ≤ n
 *            weight = binom(e + c – 1, c – 1)
 *            for all multiples m of k^e up to n
 *                a_new[m] += a_old[m / k^e] * weight
 *        a = a_new
 *
 *  The routine works for any integer type seq_t (small or arbitrary‑precision)
 *  and any unsigned 64‑bit index type idx_t.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* undefined for n = 0 */

    /* Allocate two working arrays of size n+1 (index 0 unused). */
    const size_t cap = (size_t)n + 1;
    seq_t *cur  = alloc_seq(cap);
    seq_t *next = alloc_seq(cap);

    /* initialise each entry to 0 */
    for (size_t i = 0; i < cap; ++i)
        cur[i]  = 0,
        next[i] = 0;

    cur[1] = 1;                     /* a(1) = 1 */

    /* main product loop over k */
    for (idx_t k = 2; k <= n; ++k) {
        /* start with a copy of cur */
        for (size_t i = 0; i < cap; ++i)
            next[i] = cur[i];

        /* coefficient for this k */
        const seq_t c = ((seq_t)k) * (((seq_t)k) + 1) / 2;

        /* iterate over powers of k */
        idx_t p   = k;          /* current power k^e */
        idx_t e   = 1;          /* exponent e */
        seq_t weight = c;       /* binom(e+c-1, c-1) for e=1 */

        while (p <= n) {
            /* add contribution of k^e to all multiples of p */
            for (idx_t j = p; j <= n; j += p) {
                next[j] += cur[j / p] * weight;
            }

            /* prepare for next exponent */
            if (p > n / k) break;          /* avoid overflow of p *= k */
            p *= k;
            ++e;

            /* update weight = binom(e+c-1, c-1) from previous value */
            if (e > 1) {
                /* weight *= (c + e - 1) / e; */
                weight = weight * (c + (seq_t)(e - 1)) / (seq_t)e;
            }
        }

        /* swap cur and next for the next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[n];

    free_seq(cur);
    free_seq(next);
    return result;
}
