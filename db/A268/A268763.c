/*
 * A268763  Number of nX5 binary arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two exactly once.
 * https://oeis.org/A268763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) +28*a(n-2) -62*a(n-3) -314*a(n-4) +78*a(n-5)
 *            +867*a(n-6) +6*a(n-7) -859*a(n-8) +46*a(n-9) +215*a(n-10)
 *            -8*a(n-11) -16*a(n-12) for n≥13 Initial values: a(1)=10 a(2)=105
 *            a(3)=762 a(4)=5170 a(5)=32056 a(6)=193573 a(7)=1129042
 *            a(8)=6475898 a(9)=36505596 a(10)=203462597 a(11)=112225690...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t INIT_TEN[12] = {
    (seq_t)10,  (seq_t)105,  (seq_t)762,   (seq_t)5170,
    (seq_t)32056, (seq_t)193573,(seq_t)1129042,(seq_t)6475898,
    (seq_t)36505596,(seq_t)203462597,(seq_t)1122256900,(seq_t)6140903312
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* offset starts at 1 */
    if (n <= 12) return INIT_TEN[n-1];

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    size_t new_len = (size_t)n;

    /* grow cache to hold 'n' values */
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;

        /* initialise first 12 terms if not already present */
        if (cache_len < 12) {
            size_t up = (12 < new_len) ? 12 : new_len;
            for (size_t i = cache_len; i < up; ++i)
                cache[i] = INIT_TEN[i];
            cache_len = up;
        }

        /* compute new terms using the recurrence (for indices >= 12) */
        for (size_t i = cache_len; i < new_len; ++i) {
            /* i is a 0‑based index, so a(i+1) depends on a[i]..a[i-12] */
            seq_t val = (seq_t)4   * cache[i-1];
            val += (seq_t)28  * cache[i-2];
            val -= (seq_t)62  * cache[i-3];
            val -= (seq_t)314 * cache[i-4];
            val += (seq_t)78  * cache[i-5];
            val += (seq_t)867 * cache[i-6];
            val += (seq_t)6   * cache[i-7];
            val -= (seq_t)859 * cache[i-8];
            val += (seq_t)46  * cache[i-9];
            val += (seq_t)215 * cache[i-10];
            val -= (seq_t)8   * cache[i-11];
            val -= (seq_t)16  * cache[i-12];
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
