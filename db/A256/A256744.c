/*
 * A256744  Number of (n+2)X(4+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 3 and no antidiagonal sum 3 and no row sum 0 and no column sum
 *          0.
 * https://oeis.org/A256744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (empirical):
 *   a(1) = 2065
 *   a(2) = 2512
 *   a(3) = 3752
 *   a(4) = 5888
 *   a(5) = 10360
 *   a(6) = 18688
 *   a(7) = 33616
 *   a(8) = 61568
 *   a(9) = 112928
 *   a(10)= 209024
 *   a(11)= 394816
 *   a(12)= 732800
 *   a(13)=1397888
 *   a(14)=2665088
 *
 *   for n>14:
 *   a(n) = 7*a(n-2) + 8*a(n-3) - 14*a(n-4) - 52*a(n-5) - 8*a(n-6)
 *           + 88*a(n-7) + 96*a(n-8) - 32*a(n-9) - 128*a(n-10)
 */

#include <stddef.h>  /* for size_t if needed */

seq_t a(idx_t n)
{
    /* Initial values for 1 <= n <= 14 */
    static const seq_t init[14] = {
        2065u,     /* a(1) */
        2512u,     /* a(2) */
        3752u,     /* a(3) */
        5888u,     /* a(4) */
        10360u,    /* a(5) */
        18688u,    /* a(6) */
        33616u,    /* a(7) */
        61568u,    /* a(8) */
        112928u,   /* a(9) */
        209024u,   /* a(10) */
        394816u,   /* a(11) */
        732800u,   /* a(12) */
        1397888u,  /* a(13) */
        2665088u   /* a(14) */
    };

    if (n == 0u) {
        return (seq_t)0;   /* out of bounds, return 0 */
    }
    if (n <= 14u) {
        return init[n - 1u];
    }

    /* Buffer that stores a(n-10) ... a(n-1) */
    seq_t buf[10];

    /* Initialize buffer with a(5) .. a(14) */
    for (idx_t i = 0u; i < 10u; ++i) {
        buf[i] = init[4u + i];
    }

    seq_t newVal = 0u;

    for (idx_t cur = 15u; cur <= n; ++cur) {
        /* Compute using the empirical recurrence:
         * a(cur) = 7*a(cur-2) + 8*a(cur-3) - 14*a(cur-4) - 52*a(cur-5)
         *          - 8*a(cur-6) + 88*a(cur-7) + 96*a(cur-8)
         *          - 32*a(cur-9) - 128*a(cur-10) */
        seq_t pos = 7u * buf[8u]   /* a(cur-2) */
                 + 8u * buf[7u]   /* a(cur-3) */
                 + 88u * buf[3u]  /* a(cur-7) */
                 + 96u * buf[2u]; /* a(cur-8) */

        seq_t neg = 14u * buf[6u]  /* a(cur-4) */
                 + 52u * buf[5u]  /* a(cur-5) */
                 + 8u * buf[4u]   /* a(cur-6) */
                 + 32u * buf[1u]  /* a(cur-9) */
                 + 128u * buf[0u];/* a(cur-10) */

        newVal = pos - neg;

        /* Shift buffer left by one position */
        for (int k = 0; k < 9; ++k) {
            buf[k] = buf[k + 1];
        }
        buf[9] = newVal;
    }

    return newVal;
}
