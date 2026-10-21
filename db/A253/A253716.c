/*
 * A253716  Hexagonal numbers (A000384) which are also centered heptagonal
 *          numbers (A069099).
 * https://oeis.org/A253716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4162056194*a(n-2)-a(n-4) with initial values: a(1) = 1,
 *            a(2) = 16322041, a(3) = 4145734153, a(4) = 67933251842771953
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity (number of terms) */
    static size_t size  = 0;      /* number of terms actually stored */

    if (n == 0) return 0;         /* undefined for n < 1 */

    size_t target = (size_t)n;
    if (target > cap) {
        size_t new_cap = cap ? cap << 1 : 4;
        if (new_cap < target) new_cap = target;
        /* realloc may fail; in that case we keep the old cache (undefined behaviour) */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    for (size_t i = size + 1; i <= target; ++i) {
        seq_t val;
        if (i == 1)
            val = (seq_t)1;
        else if (i == 2)
            val = (seq_t)16322041u;
        else if (i == 3)
            val = (seq_t)4145734153u;
        else if (i == 4)
            val = (seq_t)67933251842771953ULL;
        else
            val = (seq_t)4162056194u * cache[i - 3] - cache[i - 5];
        cache[i - 1] = val;
    }
    size = target;
    return cache[target - 1];
}
