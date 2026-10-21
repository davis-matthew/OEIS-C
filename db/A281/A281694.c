/*
 * A281694  Number of 2Xn 0..2 arrays with no element equal to more than one
 *          of its horizontal, diagonal or antidiagonal neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (n > 15) a(n) = 12*a(n-1) - 54*a(n-2) +
 *            136*a(n-3) - 324*a(n-4) + 720*a(n-5) - 1064*a(n-6) + 1632*a(n-7)
 *            - 2208*a(n-8) + 2048*a(n-9) - 1920*a(n-10) + 1536*a(n-11) -
 *            512*a(n-12) Initial values (1‑based): a(1)=0, a(2)=0, a(3)=6,
 *            a(4)=272, a(5)=2516, a(6)=19168, a(7)=120824, a(8...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static seq_t cache[20001] = {0};
    static size_t max_done = 0; /* number of terms that have been computed */

    if (n == 0)   /* undefined, but safe */
        return (seq_t)0;

    /* Ensure initial 15 terms are filled once */
    if (max_done < 15) {
        cache[0]  = (seq_t)0;
        cache[1]  = (seq_t)0;
        cache[2]  = (seq_t)6;
        cache[3]  = (seq_t)272;
        cache[4]  = (seq_t)2516;
        cache[5]  = (seq_t)19168;
        cache[6]  = (seq_t)120824;
        cache[7]  = (seq_t)692320;
        cache[8]  = (seq_t)3728304;
        cache[9]  = (seq_t)19162048;
        cache[10] = (seq_t)95114144;
        cache[11] = (seq_t)459530496;
        cache[12] = (seq_t)2172420416;
        cache[13] = (seq_t)10088686592;
        cache[14] = (seq_t)46160383360;
        max_done  = 15;
    }

    /* If the requested term has already been computed, return it */
    if ((size_t)(n - 1) < max_done)
        return cache[n - 1];

    /* Compute missing terms up to n using the recurrence */
    for (size_t idx = max_done; idx < (size_t)n; ++idx) {
        /* The recurrence holds for idx >= 15 (i.e., a(n) with n > 15) */
        cache[idx] =
            (seq_t)12  * cache[idx - 1] -
            (seq_t)54  * cache[idx - 2] +
            (seq_t)136 * cache[idx - 3] -
            (seq_t)324 * cache[idx - 4] +
            (seq_t)720 * cache[idx - 5] -
            (seq_t)1064 * cache[idx - 6] +
            (seq_t)1632 * cache[idx - 7] -
            (seq_t)2208 * cache[idx - 8] +
            (seq_t)2048 * cache[idx - 9] -
            (seq_t)1920 * cache[idx -10] +
            (seq_t)1536 * cache[idx -11] -
            (seq_t)512  * cache[idx -12];
    }

    max_done = (size_t)n;
    return cache[n - 1];
}
