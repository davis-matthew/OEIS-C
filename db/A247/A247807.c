/*
 * A247807  Numbers in decimal representation with distinct digits, such that
 *          in Italian and Portuguese their digits are in alphabetic order.
 * https://oeis.org/A247807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   a(n) is the nth smallest number whose decimal digits are
 *            distinct * and appear in the alphabetic order of Italian and
 *            Portuguese digit names: * 5,2,9,8,4,6,7,3,1,0
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 1022, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Number of non‑empty subsets of the 10 digits */
    static const size_t cache_len = 1023;
    static seq_t *cache = NULL;

    if (!cache) {
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;  /* Allocation failure – unlikely in tests */

        /* Digit order from the OEIS entry */
        static const int order[10] = {5,2,9,8,4,6,7,3,1,0};

        for (idx_t mask = 1; mask <= cache_len; ++mask) {
            seq_t v = 0;
            for (int p = 0; p < 10; ++p) {
                int d = order[p];
                if (mask & ((idx_t)1 << d))
                    v = v * 10 + (seq_t)d;
            }
            cache[mask - 1] = v;
        }
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);
    }

    if (n >= 1 && n <= cache_len)
        return cache[n - 1];
    else
        return 0; /* undefined for n outside 1..1023 */
}
