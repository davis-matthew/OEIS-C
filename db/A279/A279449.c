/*
 * A279449  Number of nonequivalent ways to place 5 points on an n X n square
 *          grid so that no more than 2 points are on a vertical or horizontal
 *          straight line.
 * https://oeis.org/A279449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) - 20*a(n-3) + 40*a(n-4) + 16*a(n-5) -
 *            100*a(n-6) + 44*a(n-7) + 110*a(n-8) - 110*a(n-9) - 44*a(n-10) +
 *            100*a(n-11) - 16*a(n-12) - 40*a(n-13) + 20*a(n-14) + 4*a(n-15) -
 *            5*a(n-16) + a(n-17) (offset 1) Initial values: a(1)=0, a(2)=0,
 *            a(3)=9, a(4)=306, a(5)=4361, a(6)=34059,...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 18, u64 n <= 167, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of allocated slots (index 0 unused) */
    static idx_t current_max = 0;    /* highest computed index in cache */

    /* initial values for 1..17, index 0 unused */
    const seq_t init[18] = {
        0ULL,          /* dummy 0 for index 0 */
        0ULL,          /* a(1) */
        0ULL,          /* a(2) */
        9ULL,          /* a(3) */
        306ULL,        /* a(4) */
        4361ULL,       /* a(5) */
        34059ULL,      /* a(6) */
        185181ULL,     /* a(7) */
        777280ULL,     /* a(8) */
        2710074ULL,    /* a(9) */
        8181558ULL,    /* a(10) */
        22067973ULL,   /* a(11) */
        54285858ULL,   /* a(12) */
        123791067ULL,  /* a(13) */
        264749849ULL,  /* a(14) */
        536146569ULL,  /* a(15) */
        1035584592ULL, /* a(16) */
        1919530804ULL  /* a(17) */
    };

    /* allocate or grow cache if necessary */
    if (cache == NULL) {
        /* need at least 18 slots (indices 0..17) */
        idx_t alloc = (n + 1 > 18 ? n + 1 : 18);
        cache = (seq_t *)malloc((size_t)alloc * sizeof(seq_t));
        for (idx_t i = 0; i < alloc; ++i) cache[i] = 0;
        for (idx_t i = 1; i <= 17; ++i) cache[i] = init[i];
        cache_size = alloc;
        current_max = 17;
    } else if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed, return 0 */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    if (n <= current_max) {
        return cache[n];
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t k = current_max + 1; k <= n; ++k) {
        seq_t pos = 5*cache[k-1]      /* +0 */
                  + 40*cache[k-4]
                  + 16*cache[k-5]
                  + 44*cache[k-7]
                  + 110*cache[k-8]
                  + 100*cache[k-11]
                  + 20*cache[k-14]
                  + 4*cache[k-15]
                  + cache[k-17];
        seq_t neg = 4*cache[k-2]
                  + 20*cache[k-3]
                  + 100*cache[k-6]
                  + 110*cache[k-9]
                  + 44*cache[k-10]
                  + 16*cache[k-12]
                  + 40*cache[k-13]
                  + 5*cache[k-16];
        cache[k] = pos - neg;
    }
    current_max = n;
    return cache[n];
}
