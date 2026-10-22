/*
 * A307663  a(n) = (n-1)!*(Sum_{i=1..n} Sum_{j=1..i} binomial(i,j)*i/j).
 * https://oeis.org/A307663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307663:
   a(n) = (n-1)! * Σ_{i=1..n} Σ_{j=1..i} binomial(i,j) * i/j
   Computed with the D‑finite recurrence
      2*(n+3)*(n+2)*n*(n-2)*a(n)
    - (n+3)*(5*n^2-6*n-17)*a(n+1)
    + (4 n^2-n-29)*a(n+2)
    -(n-3)*a(n+3) = 0   (n ≥ 4)
   Initial values:
      a(1)=1, a(2)=6, a(3)=41, a(4)=329,
      a(5)=3090, a(6)=33654
   The implementation uses 128‑bit temporaries when available
   to avoid overflow while computing the recurrence. */

#include <stdlib.h>

#if defined(__SIZEOF_INT128__)
/* 128‑bit signed type for temporary calculations when available */
typedef __int128 big;
#else
/* Fall back to seq_t if 128‑bit integer is not supported. */
typedef seq_t big;
#endif

seq_t a(idx_t n)
{
    /* Static memoisation cache: cache[0] holds a(1). */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap   = 32;
        cache       = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* Hard‑coded initial values */
        cache[0] = (seq_t)1;     /* a(1) */
        cache[1] = (seq_t)6;     /* a(2) */
        cache[2] = (seq_t)41;    /* a(3) */
        cache[3] = (seq_t)329;   /* a(4) */
        cache[4] = (seq_t)3090;  /* a(5) */
        cache[5] = (seq_t)33654; /* a(6) */
        cache_size = 6;
    }

    /* Return if the requested term is already computed. */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Generate terms until we have computed up to n. */
    while (cache_size < n) {
        /* k corresponds to n in the recurrence: a(k+3) from a(k),a(k+1),a(k+2). */
        idx_t k = cache_size - 2; /* Since cache_size >= 6, k >= 4. */

        seq_t a_k   = cache[k - 1]; /* a(k)   */
        seq_t a_k1  = cache[k];     /* a(k+1) */
        seq_t a_k2  = cache[k + 1]; /* a(k+2) */

        big nBig   = (big)k;
        big nPlus3 = nBig + 3;
        big nPlus2 = nBig + 2;
        big nMinus2 = nBig - 2;
        big nMinus3 = nBig - 3;

        /* Compute numerator of the recurrence in 128‑bit arithmetic. */
        big term1 = 2 * nPlus3 * nPlus2 * nBig * nMinus2 * (big)a_k;
        big term2 = nPlus3 * ( ( ( (big)5 * nBig * nBig
                                   - (big)6 * nBig
                                   - (big)17 ) * (big)a_k1 ) );
        big term3 = ( ( ( (big)4 * nBig * nBig
                          - nBig
                          - (big)29 ) * (big)a_k2 ) );
        big numerator = term1 - term2 + term3;

        seq_t a_next = (seq_t)(numerator / nMinus3);

        /* Ensure capacity for the new term */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_size] = a_next;
        cache_size++;
    }

    return cache[n - 1];
}
