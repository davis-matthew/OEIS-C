/*
 * A252133  Number of (n+2) X (2+2) 0..3 arrays with every 3 X 3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 3 4 6 or 7.
 * https://oeis.org/A252133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n‑1) + a(n‑2) + 4*a(n‑3) – a(n‑4) – 4*a(n‑5) –
 *            3*a(n‑6) + a(n‑8) + a(n‑9) (for n > 11) Initial values:
 *            a(1)=134, a(2)=156, a(3)=431, a(4)=1183, a(5)=3163, a(6)=8684,
 *            a(7)=24031, a(8)=66098, a(9)=181896, a(10)=501355, a(11)=1381325
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[12] = {
    (seq_t)0,
    (seq_t)134,
    (seq_t)156,
    (seq_t)431,
    (seq_t)1183,
    (seq_t)3163,
    (seq_t)8684,
    (seq_t)24031,
    (seq_t)66098,
    (seq_t)181896,
    (seq_t)501355,
    (seq_t)1381325
};

seq_t a(idx_t n) {
    if (n <= (idx_t)11) {
        return base[n];
    }

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* allocated size (indices 0 .. cache_len-1) */
    static idx_t computed_len = 0; /* highest index for which cache[i] is valid */

    /* Ensure we have at least 12 entries for the initial values */
    if (cache_len < 12) {
        idx_t new_len = (n + 1) > 12 ? (n + 1) : 12;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_len = new_len;
        for (idx_t i = 1; i <= 11; ++i) cache[i] = base[i];
        for (idx_t i = 12; i < cache_len; ++i) cache[i] = 0;
        computed_len = 11;
    }

    /* Expand the array if necessary */
    if (cache_len <= n) {
        idx_t old_len = cache_len;
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_len = n + 1;
        for (idx_t i = old_len; i < cache_len; ++i) cache[i] = 0;
    }

    /* Compute missing terms */
    idx_t start = computed_len + 1;
    if (start < 12) start = 12;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1]
                  + cache[i - 2]
                  + 4 * cache[i - 3]
                  - cache[i - 4]
                  - 4 * cache[i - 5]
                  - 3 * cache[i - 6]
                  + cache[i - 8]
                  + cache[i - 9];
        cache[i] = val;
    }
    computed_len = n;

    return cache[n];
}
