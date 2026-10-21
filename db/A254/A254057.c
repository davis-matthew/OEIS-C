/*
 * A254057  Sum three last digits to get next term of the sequence, starting
 *          1,2,6.
 * https://oeis.org/A254057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of the decimal digits of the last three decimal
 *            digits of the concatenation of a(n-3), a(n-2), a(n-1); a(1)=1,
 *            a(2)=2, a(3)=6
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_len_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_len_big.h"

static inline int pow10mod(int k) {
    int r = 1;
    for (int i = 0; i < k; i++)
        r = (r * 10) % 1000;
    return r;
}

static seq_t compute_next(seq_t a, seq_t b, seq_t c) {
    int rem = 0;
    seq_t parts[3] = {a, b, c};
    for (int i = 0; i < 3; i++) {
        seq_t t = parts[i];
        int len = digit_len_big(t);
        int powm = pow10mod(len);
        int tmod = (int)(t % 1000);
        rem = ((rem * powm) % 1000 + tmod) % 1000;
    }
    int d0 = rem % 10;
    int d1 = (rem / 10) % 10;
    int d2 = (rem / 100) % 10;
    return (seq_t)(d0 + d1 + d2);
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1)
        return 0; /* undefined for n < 1 */

    if (cache == NULL) {
        idx_t cap = (n > 3 ? n : 3);
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 6;
        cache_size = cap;
        for (idx_t i = 4; i <= n; i++)
            cache[i] = compute_next(cache[i - 3], cache[i - 2], cache[i - 1]);
    } else if (cache_size < n) {
        seq_t *new_cache =
            (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= n; i++)
            cache[i] = compute_next(cache[i - 3], cache[i - 2], cache[i - 1]);
        cache_size = n;
    }

    return cache[n];
}
