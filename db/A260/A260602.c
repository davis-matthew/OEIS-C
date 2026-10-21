/*
 * A260602  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000101 00010001 or 00010101.
 * https://oeis.org/A260602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = a(n-1) + 2*a(n-2) - a(n-3) + 7*a(n-4) - 10*a(n-5) + 6*a(n-6)
 *          - 5*a(n-7) - 3*a(n-8) + 6*a(n-9) + 2*a(n-10) - 8*a(n-11) + 4*a(n-12)
 *   for n > 13,
 *   a(1)=40, a(2)=58, a(3)=125, a(4)=241, a(5)=525, a(6)=1029,
 *   a(7)=2246, a(8)=4407, a(9)=9592, a(10)=18930, a(11)=41027,
 *   a(12)=81192, a(13)=175502.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values, index begins at 1 */
    static const seq_t init[14] = {
        0,                      /* unused 0‑th entry */
        (seq_t)40,   (seq_t)58,   (seq_t)125,  (seq_t)241,
        (seq_t)525,  (seq_t)1029, (seq_t)2246, (seq_t)4407,
        (seq_t)9592, (seq_t)18930, (seq_t)41027, (seq_t)81192,
        (seq_t)175502
    };

    if (n <= 13)
        return init[n];

    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* largest index actually stored in cache */

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_len = 13;
        cache = (seq_t*)malloc((cache_len + 1) * sizeof(seq_t));
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init[i];
    }

    /* grow cache if needed and compute missing terms */
    if (cache_len < n) {
        cache_len = n;
        cache = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        for (idx_t i = 14; i <= cache_len; ++i) {
            cache[i] = cache[i-1]
                   + (seq_t)2 * cache[i-2]
                   - cache[i-3]
                   + (seq_t)7 * cache[i-4]
                   - (seq_t)10 * cache[i-5]
                   + (seq_t)6 * cache[i-6]
                   - (seq_t)5 * cache[i-7]
                   - (seq_t)3 * cache[i-8]
                   + (seq_t)6 * cache[i-9]
                   + (seq_t)2 * cache[i-10]
                   - (seq_t)8 * cache[i-11]
                   + (seq_t)4 * cache[i-12];
        }
    }

    return cache[n];
}
