/*
 * A370510  Expansion of Sum_{k>=0} k! * ( x * (1+x^3) )^k.
 * https://oeis.org/A370510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} (n-3k)! * binomial(n-3k, k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_seq.h"
#include "../utils/binomial.h"

/* Static cache for factorials up to the largest n requested */
static seq_t *fact_cache = NULL;   /* array of factorials: fact_cache[i] = i! */
static size_t fact_cap  = 0;       /* allocated capacity of fact_cache */
static idx_t fact_max   = 0;       /* largest index for which fact_cache is initialized */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Ensure fact_cache has space for factorials up to n */
    if (fact_cap <= n) {
        /* Grow to at least n+1 elements */
        grow_seq(&fact_cache, &fact_cap, (size_t)(n + 1));
        /* Initialize factorial[0] if this is the first allocation */
        if (fact_max == 0 && fact_cap > 0)
            fact_cache[0] = (seq_t)1;
    }

    /* Compute missing factorials up to n */
    for (idx_t i = fact_max + 1; i <= n; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    fact_max = n;

    seq_t sum = (seq_t)0;
    idx_t kmax = n / 4;  /* floor(n/4) */

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t m = n - 3 * k;
        if (m < k) break;   /* binomial(m,k)=0 for m<k, but not needed here */
        seq_t bin = binomial(m, k);
        seq_t term = fact_cache[m] * bin;  /* (n-3k)! * binomial(n-3k, k) */
        sum += term;
    }

    return sum;
}
