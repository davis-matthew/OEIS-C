/*
 * A246475  Number of length n+3 0..4 arrays with no pair in any consecutive
 *          four terms totalling exactly 4.
 * https://oeis.org/A246475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 17, u64 n <= 38, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A246475
   Number of length n+3 arrays with entries 0..4 and no four consecutive terms
   summing to exactly 4.
   Recurrence: a(n) = 2*a(n-1) + a(n-3) + 14*a(n-4) + 3*a(n-5) + 6*a(n-6)
                     + a(n-8) + a(n-9)    for n ≥ 10
   Initial values (offset 1):
   a(1)=172, a(2)=484, a(3)=1376, a(4)=3904, a(5)=11020,
   a(6)=31104, a(7)=87888, a(8)=248568, a(9)=702724
   This implementation memoises values so that each a(n) is computed only once. */

#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/* store computed terms in a dynamically sized array                     */
/* -------------------------------------------------------------------------- */
static seq_t *cache = NULL;     /* array of computed values  */
static idx_t  cache_len  = 0;   /* highest index stored (>=1) */
static idx_t  cache_alloc = 0;  /* allocated capacity          */

/* -------------------------------------------------------------------------- */
/* initial values                                                      */
/* -------------------------------------------------------------------------- */
static const seq_t init_vals[10] = {
    (seq_t)0,      /* dummy for 0-index */
    (seq_t)172,    (seq_t)484,    (seq_t)1376,   (seq_t)3904,
    (seq_t)11020,  (seq_t)31104,  (seq_t)87888,  (seq_t)248568,
    (seq_t)702724
};

/* -------------------------------------------------------------------------- */
/* Ensure cache contains indices up to n (inclusive).                     */
/* -------------------------------------------------------------------------- */
static void ensure_capacity(idx_t n) {
    if (!cache) {                     /* first allocation */
        cache_alloc = (n > 9 ? n + 1 : 10);
        cache = (seq_t*)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failed */
        for (idx_t i = 0; i < cache_alloc; ++i)
            cache[i] = (seq_t)0;
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init_vals[i];
        cache_len = 9;
        return;
    }
    if (cache_alloc <= n) {           /* grow array */
        idx_t new_alloc = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) exit(1);
        /* initialise new slots to zero */
        for (idx_t i = cache_alloc; i < new_alloc; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_alloc = new_alloc;
    }
}

/* -------------------------------------------------------------------------- */
/*    public function : compute a(n)                                    */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0)                 /* offset 1: behaviour for n < 1 is undefined */
        return (seq_t)0;

    ensure_capacity(n);

    if (n <= cache_len)
        return cache[n];

    /* compute terms from cache_len+1 up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        /* recurrence uses indices i-1, i-3, i-4, i-5, i-6, i-8, i-9
           all of which are < i, hence already computed */
        val += (seq_t)2 * cache[i - 1];
        val += cache[i - 3];
        val += (seq_t)14 * cache[i - 4];
        val += (seq_t)3 * cache[i - 5];
        val += (seq_t)6 * cache[i - 6];
        val += cache[i - 8];
        val += cache[i - 9];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
