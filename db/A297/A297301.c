/*
 * A297301  Number of 4 X n 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 2 neighboring 1s.
 * https://oeis.org/A297301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 7*a(n-3) - 4*a(n-4) + 3*a(n-5) +
 *            a(n-6) - 5*a(n-7) + 4*a(n-8) - 2*a(n-9) - a(n-10) - 5*a(n-12) -
 *            3*a(n-13), for n > 16. Initial values a(1) .. a(16) are listed
 *            below
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Initial 16 terms of the sequence */
    static const seq_t init[16] = {
        1ULL,     1ULL,     5ULL,     10ULL,    21ULL,    50ULL,
        130ULL,   332ULL,   840ULL,   2128ULL,  5408ULL,  13772ULL,
        35102ULL, 89465ULL, 227961ULL, 580788ULL
    };

    if (n == 0) return 0;          /* Undefined for n=0, return 0. */
    if (n <= 16) return init[n-1];

    /* Buffer holding the last 13 computed values:
       buf[0] = a(i-13), buf[1] = a(i-12), ..., buf[12] = a(i-1) */
    seq_t buf[13];
    for (idx_t j = 0; j < 13; ++j)
        buf[j] = init[3 + j];      /* buf[0] = a(4), buf[12] = a(16) */

    seq_t val;                     /* Placeholder for a(i) */

    for (idx_t i = 17; i <= n; ++i) {
        /* Separate positive and negative contributions to avoid underflow. */
        seq_t pos = 0, neg = 0;
        pos += buf[12] * 4;          /* 4*a(i-1) */
        pos += buf[10] * 7;          /* 7*a(i-3) */
        pos += buf[8]  * 3;          /* 3*a(i-5) */
        pos += buf[7];               /* 1*a(i-6) */
        pos += buf[5] * 4;           /* 4*a(i-8) */

        neg += buf[11] * 6;          /* 6*a(i-2) */
        neg += buf[9]  * 4;          /* 4*a(i-4) */
        neg += buf[6]  * 5;          /* 5*a(i-7) */
        neg += buf[4]  * 2;          /* 2*a(i-9) */
        neg += buf[3];               /* 1*a(i-10) */
        neg += buf[1]  * 5;          /* 5*a(i-12) */
        neg += buf[0]  * 3;          /* 3*a(i-13) */

        val = pos - neg;             /* Final value a(i) */

        /* Shift buffer: drop a(i-13), move others left, insert a(i) at end. */
        for (idx_t j = 0; j < 12; ++j)
            buf[j] = buf[j+1];
        buf[12] = val;
    }

    return val;                     /* a(n) computed in the loop */
}
