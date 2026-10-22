/*
 * A324221  Number of connected 2n-regular loopless multigraphs with five
 *          nodes.
 * https://oeis.org/A324221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) - 2*a(n-5) +
 *            4*a(n-6) - 2*a(n-7) + a(n-8) - a(n-9) - 2*a(n-10) + 3*a(n-11) -
 *            a(n-12) Initial values: a(0)=0, a(1)=1, a(2)=6, a(3)=15,
 *            a(4)=36, a(5)=72, a(6)=139, a(7)=244, a(8)=414, a(9)=663,
 *            a(10)=1030, a(11)=1540, a(12)=2247
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial 13 terms */
    static const seq_t init[13] = {
        0ULL, 1ULL, 6ULL, 15ULL, 36ULL, 72ULL, 139ULL, 244ULL,
        414ULL, 663ULL, 1030ULL, 1540ULL, 2247ULL
    };

    if (n < 13) return init[n];

    seq_t buf[13];
    /* load initial terms into sliding window */
    for (idx_t i = 0; i < 13; ++i) buf[i] = init[i];

    seq_t next;
    for (idx_t i = 13; i <= n; ++i)
    {
        /* positive contributions */
        seq_t pos = 3 * buf[12];
        pos += buf[9];
        pos += 4 * buf[7];
        pos += buf[5];
        pos += 3 * buf[2];

        /* negative contributions (summed as a positive number) */
        seq_t neg = 2 * buf[11];
        neg += buf[10];
        neg += 2 * buf[8];
        neg += 2 * buf[6];
        neg += buf[4];
        neg += 2 * buf[3];
        neg += buf[1];

        /* next term is the difference */
        next = pos - neg;

        /* shift buffer left by one, insert new value at end */
        for (int k = 0; k < 12; ++k) buf[k] = buf[k + 1];
        buf[12] = next;
    }
    return buf[12];
}
