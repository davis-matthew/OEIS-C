/*
 * A314807  Coordination sequence Gal.6.528.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6) +
 *            a(n-7) + a(n-8) - a(n-9) + a(n-10) - a(n-11) + a(n-12) - a(n-13)
 *            + a(n-14) - a(n-15) for n > 15, a(0)=1, a(1)=5, a(2)=9, a(3)=14,
 *            a(4)=17, a(5)=22, a(6)=25, a(7)=30, a(8)=36, a(9)=40, a(10)=43,
 *            a(11)=49, a(12)=52, a(13)=57, a(14)=60, a(...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache large enough for the expected range (n <= 10000) */
    /* If your build needs a bigger range, increase MAX_N accordingly. */
    #define MAX_N 65536
    static seq_t cache[MAX_N];
    static idx_t cache_len = 0;   /* number of initialized entries */

    /* initial values for n = 0 .. 15 */
    static const seq_t init[16] = {
        (seq_t)1,   /* a(0) */
        (seq_t)5,   /* a(1) */
        (seq_t)9,   /* a(2) */
        (seq_t)14,  /* a(3) */
        (seq_t)17,  /* a(4) */
        (seq_t)22,  /* a(5) */
        (seq_t)25,  /* a(6) */
        (seq_t)30,  /* a(7) */
        (seq_t)36,  /* a(8) */
        (seq_t)40,  /* a(9) */
        (seq_t)43,  /* a(10) */
        (seq_t)49,  /* a(11) */
        (seq_t)52,  /* a(12) */
        (seq_t)57,  /* a(13) */
        (seq_t)60,  /* a(14) */
        (seq_t)65   /* a(15) */
    };

    if (n + 1 > MAX_N) {
        /* for unexpected large n, return 0 (or handle as you wish) */
        return (seq_t)0;
    }

    /* compute up to n if not already done */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 15) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5)
                                 - a(n-6) + a(n-7) + a(n-8) - a(n-9) + a(n-10)
                                 - a(n-11) + a(n-12) - a(n-13) + a(n-14) - a(n-15) */
                seq_t val =  cache[i-1] - cache[i-2] + cache[i-3] - cache[i-4] +
                            cache[i-5] - cache[i-6] + cache[i-7] + cache[i-8] -
                            cache[i-9] + cache[i-10] - cache[i-11] + cache[i-12] -
                            cache[i-13] + cache[i-14] - cache[i-15];
                cache[i] = val;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
