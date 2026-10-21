/*
 * A253521  Number of (n+1)X(5+1) 0..2 arrays with every 2X2 subblock diagonal
 *          minimum minus antidiagonal minimum nondecreasing horizontally and
 *          diagonal maximum minus antidiagonal maximum nondecreasing
 *          vertically.
 * https://oeis.org/A253521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 33*a(n-2) + 65*a(n-3) - 75*a(n-4) * +
 *            51*a(n-5) - 19*a(n-6) + 3*a(n-7) for n > 15 * Initial values (n
 *            = 1 … 15): * 42669, 169828, 256914, 370028, 673229, 1439791,
 *            3429207, * 8936967, 24799705, 71469938, 210239277, 624906937, *
 *            1866784570, 5589710182, 16755089278
 * Range:     u8 none, u16 n <= 1, u32 n <= 13, u64 n <= 33, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static const seq_t _init_vals[16] = {
    0ULL,                     /* dummy, unused 0 index */
    42669ULL,                 /* a(1)  */
    169828ULL,                /* a(2)  */
    256914ULL,                /* a(3)  */
    370028ULL,                /* a(4)  */
    673229ULL,                /* a(5)  */
    1439791ULL,               /* a(6)  */
    3429207ULL,               /* a(7)  */
    8936967ULL,               /* a(8)  */
    24799705ULL,              /* a(9)  */
    71469938ULL,              /* a(10) */
    210239277ULL,             /* a(11) */
    624906937ULL,             /* a(12) */
    1866784570ULL,            /* a(13) */
    5589710182ULL,            /* a(14) */
    16755089278ULL            /* a(15) */
};

seq_t a(idx_t n)
{
    /* Cached values: index 0 … capacity, computed holds the largest
     * index that has been filled.  We use a dynamically growing
     * array that starts with the first 15 terms. */
    static seq_t *cache = NULL;
    static idx_t capacity   = 15;
    static idx_t computed   = 15;

    if (n < 1) return 0;          /* offset 1 */

    /* Initialise cache on first use. */
    if (cache == NULL) {
        cache = (seq_t *)realloc(NULL, (capacity + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;      /* allocation failure */

        /* Load the initial 15 values. */
        for (idx_t i = 1; i <= capacity; ++i)
            cache[i] = _init_vals[i];
    }

    /* Grow cache if necessary. */
    if (n > capacity) {
        idx_t new_cap = capacity;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;              /* allocation failure */
        cache = tmp;
        /* Zero new slots. */
        for (idx_t i = capacity + 1; i <= new_cap; ++i)
            cache[i] = 0;
        capacity = new_cap;
    }

    /* Compute terms up to n if not already done. */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t t1 = 9 * cache[i - 1];
            t1 += 65 * cache[i - 3];
            t1 += 51 * cache[i - 5];
            t1 += 3  * cache[i - 7];

            seq_t t2 = 33 * cache[i - 2];
            t2 += 75 * cache[i - 4];
            t2 += 19 * cache[i - 6];

            cache[i] = t1 - t2;
        }
        computed = n;
    }

    return cache[n];
}
