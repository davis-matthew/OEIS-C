/*
 * A377329  E.g.f. satisfies A(x) = 1 - A(x)^2 * log(1 - x*A(x)^2).
 * https://oeis.org/A377329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2n+2k)!/(2n+k+1)! * |Stirling1(n,k)|
 *            (Unsigned Stirling numbers of the first kind) Sequence offset: 0
 *            (a(0) is the first term). The implementation memoises a(n) and
 *            successive rows of Stirling numbers. All calculations that stay
 *            in range of the type seq_t are done with that t...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <limits.h>

/* static state for memoisation */
static seq_t *cache      = NULL;   /* a(0)…a(n) */
static size_t cache_len  = 0;       /* allocated length of cache */
static seq_t *Sprev      = NULL;   /* previous row of Stirling numbers */
static size_t Sprev_len  = 0;       /* length of Sprev (n+1) */
static idx_t ready_n     = (idx_t)-1; /* highest n that has been processed */

/* Helper: ensure cache has room for up to n inclusive */
static void ensure_cache(idx_t n)
{
    if (cache_len <= (size_t)n) {
        size_t new_len = n + 1;
        if (new_len < 2 * cache_len) new_len = 2 * cache_len;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    /* Initialise static data if first call */
    if (ready_n == (idx_t)-1) {
        ready_n   = 0;
        cache_len = 1;
        cache     = (seq_t *)malloc(sizeof(seq_t));
        cache[0]  = (seq_t)1;          /* a(0) = 1 */

        Sprev_len = 1;
        Sprev     = (seq_t *)malloc(sizeof(seq_t));
        Sprev[0]  = (seq_t)1;          /* S(0,0) = 1 */
    }

    if ((idx_t)n <= ready_n) return cache[n];

    ensure_cache(n);

    /* Compute rows up to n */
    for (idx_t m = ready_n + 1; m <= n; ++m) {
        /* Build current Stirling row of length m+1 */
        seq_t *Scur = (seq_t *)malloc((m + 1) * sizeof(seq_t));

        /* S(m,0) = 0 except for m=0 handled above */
        if (m == 0) {
            Scur[0] = (seq_t)1;
        } else {
            Scur[0] = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k) {
                seq_t term1 = 0, term2 = 0;
                if (k <= m - 1) term1 = (seq_t)(m - 1) * Sprev[k];
                if (k - 1 <= m - 1) term2 = Sprev[k - 1];
                Scur[k] = term1 + term2;
            }
        }

        /* Compute a(m) = sum_{k=0..m} ratio(k) * Scur[k] */
        seq_t sum = (seq_t)0;
        unsigned __int128 ratio = 1; /* ratio(k) = (2m+2k)!/(2m+k+1)! */
        for (idx_t k = 0; k <= m; ++k) {
            if (k >= 2) {
                /* Update ratio via: ratio * (2m+2k-1)*(2m+2k)/(2m+k+1) */
                unsigned __int128 num1 = (unsigned __int128)(2 * (unsigned __int128)m + 2 * (unsigned __int128)k - 1);
                unsigned __int128 num2 = (unsigned __int128)(2 * (unsigned __int128)m + 2 * (unsigned __int128)k);
                unsigned __int128 den  = (unsigned __int128)(2 * (unsigned __int128)m + (unsigned __int128)k + 1);
                ratio = ratio * num1;
                ratio = ratio * num2;
                ratio = ratio / den;          /* exact division */
            }
            unsigned __int128 prod = ratio * (unsigned __int128)Scur[k];
            sum += (seq_t)prod;              /* cast back to seq_t */
        }
        cache[m] = sum;

        /* Prepare for next iteration: free old row, keep current */
        free(Sprev);
        Sprev     = Scur;
        Sprev_len = m + 1;
        ready_n   = m;
    }

    return cache[n];
}
