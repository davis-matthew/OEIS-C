/*
 * A398653  Lower (1, 1/3) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 3*a(n-3) + 3*a(n-4) * - 6*a(n-5) +
 *            3*a(n-6) + 3*a(n-7) - 6*a(n-8) * + 4*a(n-9) - a(n-10) * Initial
 *            values: a(0)=0, a(1)=1, a(2)=4, a(3)=10, a(4)=20, * a(5)=33,
 *            a(6)=51, a(7)=74, a(8)=104, a(9)=140
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t init[10] = {
        0ULL, 1ULL, 4ULL, 10ULL, 20ULL,
        33ULL, 51ULL, 74ULL, 104ULL, 140ULL
    };

    if (n < 10ULL) {
        return init[n];
    }

    seq_t buf[10];
    for (idx_t i = 0; i < 10; ++i) {
        buf[i] = init[i];
    }

    for (idx_t i = 10ULL; i <= n; ++i) {
        seq_t pos = 4 * buf[9] + 3 * buf[7] + 3 * buf[6]
                  + 3 * buf[4] + 3 * buf[3] + 4 * buf[1];
        seq_t neg = 6 * buf[8] + 6 * buf[5] + 6 * buf[2] + buf[0];
        seq_t next = pos - neg;

        /* Shift buffer left by one position */
        for (idx_t j = 0; j < 9; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[9] = next;
    }

    return buf[9];
}
