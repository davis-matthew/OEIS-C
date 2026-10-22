/*
 * A303326  Number of 4 X n 0..1 arrays with every element equal to 0, 2, 4, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=8, a(2)=2, a(3)=8, a(4)=2, a(5)=12, a(6)=10, a(7)=46,
 *            a(8)=50, a(9)=204, a(10)=290, a(11)=1034, a(12)=1682,
 *            a(13)=5458, a(14)=9802, a(15)=29582, a(16)=57122, a(17)=163166,
 *            a(18)=332930, a(19)=911976. For n > 19: a(n) =
 *            2*a(n-1)+9*a(n-2)-16*a(n-3)-25*a(n-4)+34*a(n-5)+15*a(n-6)+4*a(n-7)+34*a(n...
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 28, u64 n <= 53, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for previously computed values */
static seq_t *cache = NULL;          /* array of values a(1) .. a(max_computed) */
static idx_t cached_len = 0;         /* number of computed values (max index stored) */
static idx_t cache_cap = 0;          /* capacity of cache array (number of entries) */

/* initial 19 values of the sequence */
static const seq_t init_val[20] = {
    0,          /* dummy for 0 index */
    8,          /* a(1) */
    2,          /* a(2) */
    8,          /* a(3) */
    2,          /* a(4) */
    12,         /* a(5) */
    10,         /* a(6) */
    46,         /* a(7) */
    50,         /* a(8) */
    204,        /* a(9) */
    290,        /* a(10) */
    1034,       /* a(11) */
    1682,       /* a(12) */
    5458,       /* a(13) */
    9802,       /* a(14) */
    29582,      /* a(15) */
    57122,      /* a(16) */
    163166,     /* a(17) */
    332930,     /* a(18) */
    911976      /* a(19) */
};

seq_t a(idx_t n)
{
    /* Return cached values for n <= 19 directly */
    if (n <= 19)
        return init_val[n];

    /* Ensure cache has enough space; grow if necessary */
    if (cache_cap < n + 1) {
        /* new capacity: at least n+1, keep old capacity if larger */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort computation */
            return 0;
        }
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Copy initial values into cache if not already present */
    if (cached_len < 19) {
        for (idx_t i = 1; i <= 19; ++i)
            cache[i] = init_val[i];
        cached_len = 19;
    }

    /* Compute further terms up to n using the recurrence */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        seq_t pos = 0; /* sum of positive coefficient contributions */
        seq_t neg = 0; /* sum of negative coefficient contributions */

        /* Positive terms */
        pos += 2  * cache[i - 1];
        pos += 9  * cache[i - 2];
        pos += 34 * cache[i - 5];
        pos += 15 * cache[i - 6];
        pos += 4  * cache[i - 7];
        pos += 34 * cache[i - 8];
        pos += 20 * cache[i - 11];
        pos += 34 * cache[i - 13];
        pos += 21 * cache[i - 14];

        /* Negative terms */
        neg += 16 * cache[i - 3];
        neg += 25 * cache[i - 4];
        neg += 64 * cache[i - 9];
        neg += 42 * cache[i - 10];
        neg += 7  * cache[i - 12];
        neg += 8  * cache[i - 15];
        neg += 1  * cache[i - 16];
        neg += 6  * cache[i - 17];
        neg += 3  * cache[i - 18];

        /* Final value (pos >= neg by construction of the recurrence) */
        cache[i] = pos - neg;
    }

    cached_len = n;        /* Update the cached length */
    return cache[n];
}
