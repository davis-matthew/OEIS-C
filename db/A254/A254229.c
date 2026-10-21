/*
 * A254229  Indices of centered square numbers (A001844) which are also
 *          heptagonal numbers (A000566).
 * https://oeis.org/A254229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+322*a(n-2)-322*a(n-3)-a(n-4)+a(n-5) Initial
 *            values: a(1)=1, a(2)=46, a(3)=207, a(4)=14652, a(5)=66493
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* offset is 1; n must be >= 1 */
    if (n == 0) return 0;

    size_t target = (size_t)n - 1;         /* 0‑based index of desired value */

    if (target >= cache_cap) {
        size_t new_cap = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)46,
        (seq_t)207,
        (seq_t)14652,
        (seq_t)66493
    };

    for (size_t i = cache_len; i <= target; ++i) {
        seq_t val;
        if (i < 5) {
            val = init[i];
        } else {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            val = a1 + (seq_t)322 * a2 - (seq_t)322 * a3 - a4 + a5;
        }
        cache[i] = val;
    }
    cache_len = target + 1;
    return cache[target];
}
