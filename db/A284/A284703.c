/*
 * A284703  Number of maximal matchings in the n-prism graph.
 * https://oeis.org/A284703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+2*a(n-2)+a(n-3)-a(n-4)+2*a(n-5)+a(n-6)-a(n-7)-a(n-8)
 *            (n>8) with a(1)=1, a(2)=5, a(3)=10, a(4)=17, a(5)=51, a(6)=98,
 *            a(7)=211, a(8)=457 (offset 1)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* initial values a(1)..a(8) */
static const seq_t init_vals[8] = {
    (seq_t)1,  (seq_t)5,  (seq_t)10, (seq_t)17,
    (seq_t)51, (seq_t)98, (seq_t)211, (seq_t)457
};

/* dynamic cache for a(1)..a(max_computed) indexed 0..max_computed-1  */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;   /* number of stored terms */

/* make sure cache holds values up to n (n>=9) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;   /* already computed */

    /* allocate space for n terms */
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (tmp == NULL)
        abort();  /* allocation failure */
    cache = tmp;

    /* initialize first 8 terms on first use */
    if (cache_size == 0) {
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
        cache_size = 8;
    }

    /* compute missing terms from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* i corresponds to a(i) --> cache index i-1 */
        seq_t val = cache[i-2] + 2 * cache[i-3];   /* a(i-1)+2*a(i-2) */
        val += cache[i-4];                         /* + a(i-3) */
        val -= cache[i-5];                         /* - a(i-4) */
        val += 2 * cache[i-6];                     /* + 2*a(i-5) */
        val += cache[i-7];                         /* + a(i-6) */
        val -= cache[i-8];                         /* - a(i-7) */
        val -= cache[i-9];                         /* - a(i-8) */
        cache[i-1] = val;                          /* store a(i) */
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* value undefined for n=0, return 0 */
    if (n <= 8)
        return init_vals[n-1];
    ensure_cache(n);
    return cache[n-1];
}
