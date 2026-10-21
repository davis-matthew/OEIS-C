/*
 * A253920  Indices of centered octagonal numbers (A016754) which are also
 *          heptagonal numbers (A000566).
 * https://oeis.org/A253920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+1442*a(n-3)-1442*a(n-4)-a(n-6)+a(n-7)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1, a(2)=5, a(3)=39, a(4)=760, a(5)=6494, a(6)=55518, a(7)=1095199 */
#include <stdlib.h>

static const seq_t init[7] = {
    (seq_t)1,    /* a(1) */
    (seq_t)5,    /* a(2) */
    (seq_t)39,   /* a(3) */
    (seq_t)760,  /* a(4) */
    (seq_t)6494, /* a(5) */
    (seq_t)55518,/* a(6) */
    (seq_t)1095199/* a(7) */
};

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < 1) /* undefined for n < 1, but outside requirements */
        return (seq_t)0;

    /* For n <= 7 just return the known value */
    if (n <= 7)
        return init[(size_t)n - 1];

    /* Ensure the cache can hold up to n terms */
    size_t need = (size_t)n;
    if (need > cache_size) {
        /* Allocate or grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;

        /* Initialise the first seven entries if this is the first allocation */
        if (cache_size == 0) {
            for (size_t i = 0; i < 7; ++i)
                cache[i] = init[i];
            cache_size = 7;
        }

        /* Compute terms from cache_size+1 up to n using the recurrence */
        for (size_t i = cache_size + 1; i <= need; ++i) {
            /* i is 1‑based.  Index in cache is (i-1). */
            seq_t term_prev        = cache[i - 2]; /* a(i-1) */
            seq_t term_i_minus3    = cache[i - 4]; /* a(i-3) */
            seq_t term_i_minus4    = cache[i - 5]; /* a(i-4) */
            seq_t term_i_minus6    = cache[i - 7]; /* a(i-6) */
            seq_t term_i_minus7    = cache[i - 8]; /* a(i-7) */

            /* Compute: a(i) = a(i-1) + 1442*a(i-3) - 1442*a(i-4) - a(i-6) + a(i-7) */
            seq_t val = term_prev
                      + ((seq_t)1442) * term_i_minus3
                      - ((seq_t)1442) * term_i_minus4
                      - term_i_minus6
                      + term_i_minus7;

            cache[i - 1] = val;
        }
        cache_size = need;
    }

    return cache[(size_t)n - 1];
}
