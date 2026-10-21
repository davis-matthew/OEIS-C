/*
 * A258922  E.g.f. satisfies A(x) = 1/(3 - 2*exp(x*A(x))).
 * https://oeis.org/A258922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)!) * Sum_{k=0..n} 2^k * (n+k)! * Stirling2(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A258922.  Uses a memoised
 * O(n) incremental method for Stirling numbers of the second kind
 * to achieve an overall O(n²) running time for all requested
 * values.  All arithmetic is performed with seq_t; intermediate
 * results are held in seq_t and may overflow only when the final
 * value itself is beyond the range of seq_t (which is acceptable
 * for fixed‑width builds). */

#include <stdint.h>
#include <stdlib.h>

/* External types supplied by the build. */
extern seq_t seq_mul(seq_t a, seq_t b);  /* Optional helper; not used here. */

/* Static storage for memoised values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;          /* Number of cached entries actualy stored. */

static seq_t *S = NULL;              /* Stirling‑number array for the current n. */
static size_t S_cap = 0;

/* Helper to grow an array to at least new_cap elements. */
static void ensure_cap(seq_t **arr, size_t *cap, size_t new_cap)
{
    if (new_cap <= *cap) return;
    size_t new_cap2 = (*cap == 0) ? 64 : (*cap * 2);
    while (new_cap2 < new_cap) new_cap2 *= 2;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap2 * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failure */
    *arr = tmp;
    *cap = new_cap2;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Cache the requested value if not already present. */
    if (n < cache_len) return cache[n];

    /* Grow cache array as needed. */
    ensure_cap(&cache, &cache_cap, (size_t)n + 1);
    ensure_cap(&S,      &S_cap,      (size_t)n + 1);

    /* Initialize for n = 0 if start of cache. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        S[0] = (seq_t)1;
        cache_len = 1;
        S_cap = 1;
    }

    /* Incrementally compute all values up to n. */
    for (idx_t m = cache_len; m <= n; ++m) {
        if (m == 0) continue;       /* already set */

        /* Update Stirling array S to represent S(m, k) for k=0..m. */
        S[m] = 0;
        for (idx_t k = m; k >= 1; --k) {
            S[k] = (seq_t)k * S[k] + S[k-1];
        }
        S[0] = 0;   /* S(m,0)=0 for m>0 */

        /* Compute a(m). */
        seq_t sum  = 0;
        seq_t prod = 1;   /* product (n+2)*(n+3)*...*(n+k) */
        seq_t pow2 = 1;   /* will hold 2^k */

        for (idx_t k = 1; k <= m; ++k) {
            pow2 *= (seq_t)2;                      /* 2^k */
            if (k > 1) prod *= (seq_t)(m + (idx_t)k);  /* multiply by n+k */

            seq_t term = S[k];
            term *= pow2;
            term *= prod;
            sum += term;
        }

        cache[m] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
