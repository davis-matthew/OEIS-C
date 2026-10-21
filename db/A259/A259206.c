/*
 * A259206  G.f. satisfies: A(x) = (1-x^2) * (1 + x*A(x)^2).
 * https://oeis.org/A259206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A259206
 *
 *  Recurrence (n >= 7):
 *      (n+1)*a(n) = 2*(2*n-1)*a(n-1) + (n+1)*a(n-2)
 *                   - 6*(2*n-5)*a(n-3) + 6*(2*n-9)*a(n-5)
 *                   - 2*(2*n-13)*a(n-7)
 *
 *  Initial values:
 *      a(0)=1  a(1)=1  a(2)=1  a(3)=2  a(4)=4  a(5)=10  a(6)=26
 *
 *  The function is memoised; calls may be in arbitrary order.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;      /* a(0)...a(last) */
static size_t cache_cap = 0;     /* allocated length */
static idx_t  cache_last = 0;    /* highest index stored */

/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
/* make sure that cache has values 0..n (inclusive) */
{
    /* Initialise cache if not yet allocated */
    if (cache == NULL) {
        /* start with enough space for at least n+1 elements, round up */
        size_t init_cap = n + 1;
        if (init_cap < 16) init_cap = 16;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) abort();

        cache_cap  = init_cap;
        /* fill the initial known values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)10;
        cache[6] = (seq_t)26;
        cache_last = 6;
    }
    /* Grow the array if we need more space */
    else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Compute any missing terms via the recurrence */
    if (n > cache_last) {
        for (idx_t k = cache_last + 1; k <= n; ++k) {
            /* grab required previous values */
            const seq_t a1 = cache[k - 1];
            const seq_t a2 = cache[k - 2];
            const seq_t a3 = cache[k - 3];
            const seq_t a5 = cache[k - 5];
            const seq_t a7 = cache[k - 7];

            const seq_t two_k_minus_one   = (seq_t)2 * (seq_t)k - (seq_t)1;
            const seq_t two_k_minus_five  = (seq_t)2 * (seq_t)k - (seq_t)5;
            const seq_t two_k_minus_nine  = (seq_t)2 * (seq_t)k - (seq_t)9;
            const seq_t two_k_minus_thirteen = (seq_t)2 * (seq_t)k - (seq_t)13;

            const seq_t rhs = (seq_t)2 * two_k_minus_one * a1
                            + (seq_t)(k + 1) * a2
                            - (seq_t)6 * two_k_minus_five * a3
                            + (seq_t)6 * two_k_minus_nine * a5
                            - (seq_t)2 * two_k_minus_thirteen * a7;

            cache[k] = rhs / ((seq_t)(k + 1));
        }
        cache_last = n;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
/* Return a(n).  For n <= 6 the constant table is used directly. */
{
    /* handle the first seven terms without caching */
    if (n <= 6) {
        static const seq_t init[7] = {
            (seq_t)1,   /* a(0) */
            (seq_t)1,   /* a(1) */
            (seq_t)1,   /* a(2) */
            (seq_t)2,   /* a(3) */
            (seq_t)4,   /* a(4) */
            (seq_t)10,  /* a(5) */
            (seq_t)26   /* a(6) */
        };
        return init[n];
    }

    /* For n >= 7 we rely on the memoised recurrence */
    ensure_cache(n);
    return cache[n];
}
