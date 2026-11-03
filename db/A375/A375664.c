/*
 * A375664  Expansion of e.g.f. 1 / (1 - x * (exp(x^2) - 1))^2.
 * https://oeis.org/A375664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/2⌋} (n-2k+1)! * S(k,n-2k) / k!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where S(k,m) is the Stirling number of the second kind. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free(seq_t *p) { mem_free(p); }

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute factorials up to n.  fact[i] = i! */
    idx_t fact_size = n + 1;                /* indices 0 .. n */
    seq_t *fact = seq_alloc_new_array(fact_size);
    if (!fact)
        exit(1);
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Prepare two rows for Stirling numbers S(k,m) */
    idx_t kmax = n / 2;                     /* floor(n/2) */
    idx_t row_len = kmax + 1;               /* indices 0 .. kmax */
    seq_t *row1 = seq_alloc_new_array(row_len);
    seq_t *row2 = seq_alloc_new_array(row_len);
    if (!row1 || !row2) {
        if (row1) seq_free(row1);
        if (row2) seq_free(row2);
        seq_free(fact);
        exit(1);
    }

    /* S(0,0) = 1 */
    row1[0] = (seq_t)1;

    seq_t result = (seq_t)0;

    /* Iterate over k = 1 .. kmax, computing each row */
    for (idx_t k = 1; k <= kmax; ++k)
    {
        seq_t *prev = (k & 1) ? row2 : row1;
        seq_t *curr = (k & 1) ? row1 : row2;

        /* curr[0] = 0 */
        curr[0] = (seq_t)0;

        /* middle entries: S(k,m) = S(k-1,m-1) + m * S(k-1,m)  for 1 <= m < k */
        for (idx_t m = 1; m < k; ++m)
            curr[m] = prev[m - 1] + mul_small(prev[m], (unsigned)m);

        /* S(k,k) = 1 */
        curr[k] = (seq_t)1;

        /* If m = n - 2*k is a valid Stirling index, add contribution */
        idx_t m = n - 2 * k;
        if (m <= k)
        {
            /* Compute term = (n! / k!) * (m+1)! * S(k,m) */
            seq_t term = fact[n] / fact[k];          /* n! / k!  (exact) */
            term *= fact[m + 1];                     /* * (m+1)! */
            term *= curr[m];                         /* * S(k,m) */
            result += term;
        }
    }

    /* Clean up */
    seq_free(fact);
    seq_free(row1);
    seq_free(row2);

    return result;
}
