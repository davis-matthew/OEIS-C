/*
 * A274854  Number of n X 4 0..2 arrays with no element equal to any value at
 *          offset (-1,0) (0,-1) or (-2,-2) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 10*a(n-3) - 6*a(n-4) + a(n-5) for
 *            n>7, with a(1)=4, a(2)=27, a(3)=43, a(4)=118, a(5)=330,
 *            a(6)=923, a(7)=2585
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based cache of computed terms */
static idx_t  cache_size = 0;        /* highest index stored in cache */

/* initialise cache with the first seven terms */
static void init_cache(void)
{
    if (cache != NULL) return;
    /* indices 0..7; we keep 0 unused for simplicity */
    static const seq_t init_vals[8] =
    { 0,
      4ULL,    /* a(1) */
      27ULL,   /* a(2) */
      43ULL,   /* a(3) */
      118ULL,  /* a(4) */
      330ULL,  /* a(5) */
      923ULL,  /* a(6) */
      2585ULL  /* a(7) */
    };
    /* allocate space for indices 0..7 (total 8 elements) */
    cache = (seq_t *)malloc(8 * sizeof(seq_t));
    for (idx_t i = 0; i <= 7; ++i) cache[i] = init_vals[i];
    cache_size = 7;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence is defined for n ≥ 1 */

    init_cache();                 /* ensure cache is ready */

    if (n <= cache_size)          /* already computed */
        return cache[n];

    /* grow cache if needed */
    idx_t new_cap = cache_size + 1;
    while (new_cap < n + 1) new_cap <<= 1;   /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* unlikely in normal use */
    cache = tmp;

    /* compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        /* recurrence: a(i) = 5*a(i-1) - 9*a(i-2) + 10*a(i-3) - 6*a(i-4) + a(i-5) */
        seq_t val = cache[i - 1] * 5ULL;   /* 5*a(i-1) */
        val -= cache[i - 2] * 9ULL;        /* -9*a(i-2)  */
        val += cache[i - 3] * 10ULL;       /* +10*a(i-3) */
        val -= cache[i - 4] * 6ULL;        /* -6*a(i-4)  */
        val += cache[i - 5];               /* +a(i-5)    */
        cache[i] = val;
    }

    cache_size = n;                            /* update current size */
    return cache[n];
}
