/*
 * A394276  Number of induced cubes in the n-triangular honeycomb queen graph.
 * https://oeis.org/A394276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 7*a(n-2) - 21*a(n-4) + 35*a(n-6) - 35*a(n-8) 
 *              + 21*a(n-10) - 7*a(n-12) + a(n-14)    with offset 1
 * Initial values:
 * a(1)=1, a(2)=6, a(3)=21, a(4)=67, a(5)=168, a(6)=369, a(7)=733,
 * a(8)=1350, a(9)=2328, a(10)=3823, a(11)=6009, a(12)=9114,
 * a(13)=13399, a(14)=19185.
 */

#include <stddef.h>  /* size_t, ptrdiff_t */
#include <stdlib.h>  /* malloc, realloc */

static seq_t *cache = NULL;          /* cached values             */
static size_t cache_len = 0;         /* number of values currently cached */
static size_t cache_cap = 0;         /* allocated size of cache      */

/* true initial 14 terms, as constants */
static const seq_t init_terms[14] = {
    (seq_t)1,   (seq_t)6,   (seq_t)21,   (seq_t)67,
    (seq_t)168, (seq_t)369, (seq_t)733,  (seq_t)1350,
    (seq_t)2328,(seq_t)3823,(seq_t)6009, (seq_t)9114,
    (seq_t)13399,(seq_t)19185
};

/* public function implementing the sequence */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n<1 – return 0 safely */
        return (seq_t)0;

    /* ensure cache has enough capacity */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                /* allocation failed – give up */
            return (seq_t)0;
        cache    = tmp;
        cache_cap = new_cap;
    }

    /* compute all terms up to n that are not yet computed */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 14) {                      /* copy initial terms */
            cache[i - 1] = init_terms[i - 1];
        } else {                            /* recurrence formula */
            cache[i - 1] =
                (seq_t)7  * cache[i - 3]   /* a(i-2) */
              - (seq_t)21 * cache[i - 5]   /* a(i-4) */
              + (seq_t)35 * cache[i - 7]   /* a(i-6) */
              - (seq_t)35 * cache[i - 9]   /* a(i-8) */
              + (seq_t)21 * cache[i - 11]  /* a(i-10) */
              - (seq_t)7  * cache[i - 13]  /* a(i-12) */
              +        cache[i - 15];      /* a(i-14) */
        }
    }

    cache_len = (size_t)n;         /* we now have all values up to n */
    return cache[n - 1];
}
