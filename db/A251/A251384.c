/*
 * A251384  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 2 3 4 5 or 6.
 * https://oeis.org/A251384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*a(n-1) - 21*a(n-2) - 119*a(n-3) + 66*a(n-4) +
 *            72*a(n-5) - 32*a(n-6) with initial values: a(1) = 571 a(2) =
 *            12457 a(3) = 271393 a(4) = 5914339 a(5) = 128887759 a(6) =
 *            2808783981
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* No defined behaviour for n==0; return 0 as placeholder */
    if (n == 0) return 0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t   size = 0;   /* actual number of terms stored */
    static size_t  cap  = 0;   /* allocated capacity */

    /* Grow the cache if necessary */
    if (n > size) {
        idx_t new_size = n;
        if (cap < (size_t)new_size) {
            size_t new_cap = cap ? cap : 16;
            while (new_cap < (size_t)new_size) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)           val = (seq_t)571;
            else if (i == 2)      val = (seq_t)12457;
            else if (i == 3)      val = (seq_t)271393;
            else if (i == 4)      val = (seq_t)5914339;
            else if (i == 5)      val = (seq_t)128887759;
            else if (i == 6)      val = (seq_t)2808783981;
            else {
                seq_t t1 = (seq_t)23 * cache[i-2];
                seq_t t2 = (seq_t)21 * cache[i-3];
                seq_t t3 = (seq_t)119 * cache[i-4];
                seq_t t4 = (seq_t)66  * cache[i-5];
                seq_t t5 = (seq_t)72  * cache[i-6];
                seq_t t6 = (seq_t)32  * cache[i-7];
                val = t1 - t2 - t3 + t4 + t5 - t6;
            }
            cache[i-1] = val;
        }
        size = new_size;
    }
    return cache[n-1];
}
