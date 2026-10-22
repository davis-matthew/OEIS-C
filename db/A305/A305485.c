/*
 * A305485  Number of nX4 0..1 arrays with every element unequal to 1, 2, 3, 6
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 67, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) +4*a(n-3) -26*a(n-4) +29*a(n-6)
   +55*a(n-7) -18*a(n-8) -111*a(n-9) -15*a(n-10) +60*a(n-11)
   +72*a(n-12) -14*a(n-13) -36*a(n-14) -13*a(n-15)
   +5*a(n-16) +6*a(n-17)  for n > 22
   Initial terms (a(1) .. a(22)):
   2, 33, 27, 65, 195, 125, 268, 1176, 805, 1681,
   7276, 5844, 10771, 45897, 42168, 71400, 291373,
   302289, 481228, 1860822, 2143789, 3279467 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Table of the first 22 terms (index 0 -> a(1)). */
    static const seq_t init[22] = {
        2ULL, 33ULL, 27ULL, 65ULL, 195ULL, 125ULL,
        268ULL, 1176ULL, 805ULL, 1681ULL, 7276ULL, 5844ULL,
        10771ULL, 45897ULL, 42168ULL, 71400ULL, 291373ULL,
        302289ULL, 481228ULL, 1860822ULL, 2143789ULL, 3279467ULL
    };

    if (n <= 22ULL)
        return init[n - 1ULL];

    /* Buffer holding a(k-1) .. a(k-17) for the current step k. */
    seq_t buf[17];

    /* Initialize buffer with a(22), a(21), … , a(6). */
    buf[0]  = init[21]; /* a(22) */
    buf[1]  = init[20]; /* a(21) */
    buf[2]  = init[19]; /* a(20) */
    buf[3]  = init[18]; /* a(19) */
    buf[4]  = init[17]; /* a(18) */
    buf[5]  = init[16]; /* a(17) */
    buf[6]  = init[15]; /* a(16) */
    buf[7]  = init[14]; /* a(15) */
    buf[8]  = init[13]; /* a(14) */
    buf[9]  = init[12]; /* a(13) */
    buf[10] = init[11]; /* a(12) */
    buf[11] = init[10]; /* a(11) */
    buf[12] = init[9];  /* a(10) */
    buf[13] = init[8];  /* a(9) */
    buf[14] = init[7];  /* a(8) */
    buf[15] = init[6];  /* a(7) */
    buf[16] = init[5];  /* a(6) */

    for (idx_t i = 23ULL; i <= n; ++i) {
        /* Compute the positive part of the recurrence */
        seq_t pos = 0;
        pos += 3ULL  *buf[0];
        pos += 4ULL  *buf[2];
        pos += 29ULL *buf[5];
        pos += 55ULL *buf[6];
        pos += 60ULL *buf[10];
        pos += 72ULL *buf[11];
        pos += 5ULL  *buf[15];
        pos += 6ULL  *buf[16];

        /* Compute the negative part of the recurrence */
        seq_t neg = 0;
        neg += 26ULL *buf[3];
        neg += 18ULL *buf[7];
        neg += 111ULL*buf[8];
        neg += 15ULL *buf[9];
        neg += 14ULL *buf[12];
        neg += 36ULL *buf[13];
        neg += 13ULL *buf[14];

        /* The new term is the difference of the positives and negatives */
        seq_t new_val = pos - neg;   /* safe; pos >= neg for all valid n */

        /* Shift the buffer (oldest element discarded) and insert new_val */
        for (int k = 16; k > 0; --k)
            buf[k] = buf[k-1];
        buf[0] = new_val;
    }

    return buf[0]; /* a(n) */
}
