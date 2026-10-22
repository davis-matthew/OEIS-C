/*
 * A303725  Number of nX6 0..1 arrays with every element unequal to 0, 1, 2 or
 *          3 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) -a(n-4) -a(n-5) +a(n-6) -a(n-7) -a(n-8)
 *            +2*a(n-9) for n>15 Initial values: a(1)=32, a(2)=293, a(3)=151,
 *            a(4)=221, a(5)=345, a(6)=569, a(7)=894, a(8)=1402, a(9)=2341,
 *            a(10)=3998, a(11)=6796, a(12)=11609, a(13)=19890, a(14)=34076,
 *            a(15)=58545
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 35, u64 n <= 76, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* C++ compatibility: idx_t and seq_t are defined by the build system.  */
seq_t a(idx_t n)
{
    /* Base values directly from the OEIS entry.  Use 64‑bit constants
       and cast to seq_t to avoid any dependance on the actual type.   */
    static const uint64_t base_int[16] = {
        0ULL,          /* dummy for 0 index */
        32ULL,        /* a(1) */
        293ULL,       /* a(2) */
        151ULL,       /* a(3) */
        221ULL,       /* a(4) */
        345ULL,       /* a(5) */
        569ULL,       /* a(6) */
        894ULL,       /* a(7) */
        1402ULL,      /* a(8) */
        2341ULL,      /* a(9) */
        3998ULL,      /* a(10) */
        6796ULL,      /* a(11) */
        11609ULL,     /* a(12) */
        19890ULL,     /* a(13) */
        34076ULL,     /* a(14) */
        58545ULL      /* a(15) */
    };

    /* Return a small n directly.  */
    if (n <= 15) {
        return (seq_t)base_int[(idx_t)n];
    }

    /* Static cache of already computed values.  */
    static seq_t *cache = NULL;          /* 1‑based indexing: cache[i] = a(i) */
    static idx_t cache_size = 0;         /* largest n for which cache[n] is valid */
    static idx_t cache_capacity = 0;     /* allocated size (index 0…capacity-1) */

    /* Allocate cache the first time or enlarge it if required.  */
    if (n + 1 > cache_capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        /* In the unlikely event of allocation failure we just abort;  */
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = n + 1;
    }

    /* Fill the cache with the known initial values if not yet done.  */
    if (cache_size < 15) {
        for (idx_t i = 1; i <= 15; ++i) {
            cache[i] = (seq_t)base_int[i];
        }
        cache_size = 15;
    }

    /* Compute further values using the recurrence.  */
    if (cache_size < n) {
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* The recurrence uses the 9 preceding terms.  */
            cache[i] = 2 * cache[i - 1]
                       - cache[i - 4]
                       - cache[i - 5]
                       + cache[i - 6]
                       - cache[i - 7]
                       - cache[i - 8]
                       + 2 * cache[i - 9];
        }
        cache_size = n;
    }

    return cache[n];
}
