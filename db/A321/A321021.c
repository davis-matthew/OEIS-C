/*
 * A321021  a(0)=0, a(1)=1; thereafter a(n) = a(n-2)+a(n-1), keeping just the
 *          digits that appear exactly once.
 * https://oeis.org/A321021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-2)+a(n-1) with a(0)=0, a(1)=1; after the sum keep
 *            only digits that appear exactly once in the decimal
 *            representation
 * Range:     u8 n <= 15, u16 n <= 250, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamically growing cache that works in
   both C (C11) and C++ (C++17).  In C++ the cache is managed with
   operator new/delete; in C it is managed with malloc/realloc.  */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache       = NULL;
    static size_t allocated   = 0;   /* allocated capacity of cache   */
    static idx_t  computed    = 1;   /* highest index whose value is valid (>=1) */

    /* --- initialisation ------------------------------------------- */
    if (cache == NULL) {
        /* reserve space for a(0) and a(1) */
#ifdef __cplusplus
        cache = new seq_t[2];
#else
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
#endif
        cache[0] = 0;
        cache[1] = 1;
        allocated = 2;
    }

    /* --- extend cache to accommodate n, if necessary -------------- */
    if (n > computed) {
        /* increase capacity if needed */
        if (n + 1 > (idx_t)allocated) {
            size_t new_alloc = allocated ? allocated * 2 : 2;
            while (new_alloc <= (size_t)n) new_alloc *= 2;
            if (new_alloc < (size_t)(n + 1)) new_alloc = (size_t)(n + 1);
#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_alloc];
            for (size_t i = 0; i < allocated; ++i) tmp[i] = cache[i];
            delete[] cache;
            cache = tmp;
#else
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_alloc);
            cache = tmp;
#endif
            allocated = new_alloc;
        }

        /* compute values from computed+1 up to n */
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum = cache[i - 2] + cache[i - 1];

            /* Count digit frequencies and store digits in reverse order */
            unsigned char counts[10] = {0};
            unsigned char rev[256];  /* enough for any reasonable n */
            size_t rev_len = 0;

            seq_t t = sum;
            if (t == 0) {
                counts[0] = 1;
                rev[rev_len++] = 0;
            } else {
                while (t > 0) {
                    unsigned d = (unsigned)(t % 10);
                    counts[d] += 1;
                    rev[rev_len++] = (unsigned char)d;
                    t /= 10;
                }
            }

            /* Reconstruct number keeping only unique digits */
            seq_t res = 0;
            for (int j = (int)rev_len - 1; j >= 0; --j) {
                unsigned d = rev[j];
                if (counts[d] == 1) {
                    res = res * 10 + d;
                }
            }
            cache[i] = res;
        }
        computed = n;
    }

    return cache[n];
}
