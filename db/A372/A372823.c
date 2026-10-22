/*
 * A372823  Sequence formed as follows: for each k >= 0, insert between 3^k
 *          and 3^(k+1) the least power of 2 that is in the interval [3^k,
 *          3^(k+1)], and then arrange the resulting numbers in nondecreasing
 *          order.
 * https://oeis.org/A372823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n/2) if n is even a(n) = 2^ceil(log2(3^((n-1)/2))) if
 *            n is odd Implements OEIS A372823
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of powers of 3 */
    static seq_t *pow3_cache = NULL;
    static idx_t pow3_len = 0;   /* number of entries filled in cache   */
    static idx_t pow3_cap = 0;   /* current allocated capacity          */

    /* k = floor(n/2) identifies the power of 3 belonging to index n */
    idx_t k = n >> 1;

    /* Grow the cache if necessary */
    if (k >= pow3_len) {
        idx_t new_cap = pow3_cap ? pow3_cap << 1 : 1;
        while (new_cap <= k) new_cap <<= 1;                /* geometric growth */
        pow3_cache = (seq_t *)realloc(pow3_cache, new_cap * sizeof(seq_t));
        pow3_cap = new_cap;
        for (idx_t i = pow3_len; i <= k; ++i) {
            if (i == 0)
                pow3_cache[i] = (seq_t)1;                  /* 3^0 = 1 */
            else
                pow3_cache[i] = pow3_cache[i-1] * (seq_t)3;
        }
        pow3_len = k + 1;                                /* we now have 0..k inclusive */
    }

    /* Retrieve 3^k */
    seq_t pow3 = pow3_cache[k];

    /* Even index: a(n) = 3^k */
    if ((n & 1) == 0)
        return pow3;

    /* Odd index: a(n) = smallest power of 2 >= 3^k */
    seq_t pow2 = (seq_t)1;
    while (pow2 < pow3)
        pow2 = pow2 * (seq_t)2;
    return pow2;
}
