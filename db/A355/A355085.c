/*
 * A355085  E.g.f. A(x) satisfies A(x) = 1 + log(1+x) * A(3*x).
 * https://oeis.org/A355085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A355085
    a(0) = 1
    a(n) = sum_{k=1..n} (-1)^(k-1) * 3^(n-k) * (k-1)! * C(n,k) * a(n-k)
    (where C(n,k) is the binomial coefficient)
*/

#include <stddef.h>
#include <stdlib.h>

/* Dynamic caching for the sequence values, factorials and powers of 3 */
static seq_t *a_cache      = NULL;   /* a_cache[i] = a(i)          */
static seq_t *fact_cache   = NULL;   /* fact_cache[i] = i!        */
static seq_t *pow3_cache   = NULL;   /* pow3_cache[i] = 3^i       */
static size_t cache_size   = 0;       /* number of elements stored
                                        (always == size of the arrays) */

/* Extend the caches so that indices 0..new_n are valid */
static void extend_cache(size_t new_n)
{
    if (new_n + 1 <= cache_size)
        return;                            /* already big enough */

    size_t new_size = new_n + 1;

    /* Allocate new storage */
    seq_t *new_a  = (seq_t *)malloc(new_size * sizeof(seq_t));
    seq_t *new_f  = (seq_t *)malloc(new_size * sizeof(seq_t));
    seq_t *new_p3 = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_a || !new_f || !new_p3)
        return;  /* allocation failure – leave caches untouched */

    /* Copy existing data */
    for (size_t i = 0; i < cache_size; ++i) {
        new_a[i]  = a_cache[i];
        new_f[i]  = fact_cache[i];
        new_p3[i] = pow3_cache[i];
    }

    /* Initialize freshly needed factorials and powers of 3 */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            new_f[i]  = (seq_t)1;   /* 0! = 1 */
            new_p3[i] = (seq_t)1;   /* 3^0 = 1 */
        } else {
            new_f[i]  = new_f[i-1] * (seq_t)i;
            new_p3[i] = new_p3[i-1] * (seq_t)3;
        }
    }

    /* Compute new sequence terms */
    for (size_t n = cache_size; n < new_size; ++n) {
        if (n == 0) {
            new_a[0] = (seq_t)1;
            continue;
        }

        seq_t pos = (seq_t)0;   /* positive contributions */
        seq_t neg = (seq_t)0;   /* negative contributions */

        /* binomial(n,k) is updated incrementally */
        seq_t binom = (seq_t)1;  /* C(n,0) = 1 */

        for (size_t k = 1; k <= n; ++k) {
            /* Update binomial coefficient: C(n,k) = C(n,k-1) * (n-k+1) / k */
            binom = binom * (seq_t)(n - k + 1);
            binom = binom / (seq_t)k;

            /* term = a(n-k) * (k-1)! * C(n,k) * 3^(n-k) */
            seq_t term = new_a[n - k];
            term = term * new_f[k - 1];
            term = term * binom;
            term = term * new_p3[n - k];

            /* Add or subtract the term */
            if (k & 1)       /* k odd -> + */
                pos += term;
            else              /* k even -> - */
                neg += term;
        }

        new_a[n] = pos - neg;  /* pos >= neg for this sequence */
    }

    /* Free old memory */
    if (a_cache)      free(a_cache);
    if (fact_cache)   free(fact_cache);
    if (pow3_cache)   free(pow3_cache);

    /* Install new caches */
    a_cache      = new_a;
    fact_cache   = new_f;
    pow3_cache   = new_p3;
    cache_size   = new_size;
}

/* Public interface ------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure all values up to n are computed */
    if ((size_t)n >= cache_size)
        extend_cache((size_t)n);

    return a_cache[(size_t)n];
}
