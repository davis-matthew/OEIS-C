/*
 * A256817  Number of length n+2 0..1 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A256817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 5*n^4 + 45*n^3 - 5*n^2 + 554*n + 360) / 120
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the polynomial representation of OEIS sequence A256817. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute the quintic polynomial using 128‑bit arithmetic to avoid overflow. */
    __int128 N = ( __int128 ) n;
    /* Terms of the numerator: n^5 + 5*n^4 + 45*n^3 - 5*n^2 + 554*n + 360 */
    __int128 num = N * N * N * N * N;                      /* n^5          */
    num += 5 * N * N * N * N;                              /* +5*n^4       */
    num += 45 * N * N * N;                                 /* +45*n^3      */
    num -= 5 * N * N;                                      /* -5*n^2       */
    num += 554 * N;                                        /* +554*n       */
    num += 360;                                            /* +360         */

    /* Final division by 120 yields the required integer value. */
    seq_t result = (seq_t)(num / 120);
    return result;
}
