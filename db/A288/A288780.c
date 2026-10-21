/*
 * A288780  Zero together with the row sums of A288778.
 * https://oeis.org/A288780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=1}^{n‑1} k! (a(0)=a(1)=0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached factorials: fact[i] = i!   (0 ≤ i ≤ cache_max) */
static seq_t *fact   = NULL;
static seq_t *sumfact = NULL;   /* sumfact[i] = Σ_{k=1}^{i} k! */
static size_t cache_max = 0;    /* highest index with data filled */

/* Ensure fact[0..m] and sumfact[0..m] are computed */
static void ensure_cache(idx_t m)
{
    if (m < (idx_t)cache_max) /* already covered */
        return;

    size_t target = (size_t)m + 1; /* array size needed */

    /* first allocation */
    if (fact == NULL) {
        fact = (seq_t*)malloc(target * sizeof(seq_t));
        sumfact = (seq_t*)malloc(target * sizeof(seq_t));
        if (fact == NULL || sumfact == NULL) {
            /* allocation failure – abort: undefined behaviour for this simple build */
            exit(1);
        }
        fact[0]    = (seq_t)1;   /* 0! = 1 */
        sumfact[0] = (seq_t)0;   /* empty sum */
        cache_max = 0;
    }
    /* grow arrays if necessary */
    else if (target > cache_max + 1) {
        fact = (seq_t*)realloc(fact, target * sizeof(seq_t));
        sumfact = (seq_t*)realloc(sumfact, target * sizeof(seq_t));
        if (fact == NULL || sumfact == NULL) {
            exit(1);
        }
    }

    /* compute new entries */
    for (size_t i = cache_max + 1; i <= (size_t)m; ++i) {
        /* i is never zero here, because m ≥ cache_max+1 ≥ 1 */
        seq_t i_seq = (seq_t)i;
        fact[i] = fact[i - 1] * i_seq;                    /* i!  */
        sumfact[i] = sumfact[i - 1] + fact[i];            /* Σ_{k=1}^i k! */
    }
    cache_max = (size_t)m;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;        /* a(0)=a(1)=0 */

    idx_t m = n - 1;            /* we need Σ_{k=1}^{n-1} k!   */
    ensure_cache(m);

    return (seq_t)n * sumfact[m];
}
