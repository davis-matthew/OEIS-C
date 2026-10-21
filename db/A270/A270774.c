/*
 * A270774  a(n) = (A005706(n) - A194459(n))/5.
 * https://oeis.org/A270774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A005706(n) - A194459(n))/5
 * Range:     u8 n <= 42, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A005706(n) is defined by b(0)=1 and b(n)=b(n-1)+b(floor(n/5)) */
/*  A194459(n) is the product of (digit+1) for the base‑5 digits of n. */

#include <stdlib.h>      /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /*  Memoised computation of b(n) = A005706(n)                           */
    /* ------------------------------------------------------------------ */
    static seq_t *b_cache = NULL;   /* array holding b(i) values */
    static size_t cache_cap = 0;    /* allocated size of b_cache */
    static idx_t b_max_computed = 0;/* largest n for which b_cache[n] is valid */

    /* Initialise cache on first use */
    if (b_cache == NULL) {
        cache_cap = 1;
        b_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!b_cache) abort();      /* out of memory */
        b_cache[0] = (seq_t)1;      /* b(0)=1 */
        b_max_computed = 0;
    }

    /* Grow cache if necessary */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap) new_cap = cache_cap * 2; /* safety
                                                             but should not
                                                                happen */
        seq_t *tmp = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        b_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values of b(i) up to n */
    for (idx_t i = b_max_computed + 1; i <= n; ++i) {
        seq_t prev = b_cache[i - 1];
        idx_t div5 = i / 5;
        seq_t add  = b_cache[div5];
        b_cache[i] = prev + add;
    }
    b_max_computed = n;

    seq_t b_n = b_cache[n];

    /* ------------------------------------------------------------------ */
    /*  Compute c(n) = product (digit+1) for base‑5 representation of n   */
    /* ------------------------------------------------------------------ */
    seq_t prod = (seq_t)1;
    idx_t x = n;
    while (x > 0) {
        uint64_t digit = x % 5;      /* digit is small, fits in 64 bits */
        prod *= (seq_t)(digit + 1);
        x /= 5;
    }

    /* ------------------------------------------------------------------ */
    /*  Return a(n) = (b(n) - c(n))/5                                   */
    /* ------------------------------------------------------------------ */
    return (b_n - prod) / (seq_t)5;
}
