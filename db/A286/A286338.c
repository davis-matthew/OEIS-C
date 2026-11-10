/*
 * A286338  Positions of 1 in A286336; complement of A286337.
 * https://oeis.org/A286338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A286336 (A286336 = Thue-Morse
 *            word with non-overlapping 1101 replaced by 0); computed by
 *            streaming scan
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9241 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    unsigned char buf[4];
    int bufSize = 0;
    idx_t i = 0;          /* Thue-Morse index */
    idx_t outIdx = 0;     /* outputs produced so far */
    idx_t ones = 0;       /* ones seen so far */

    for (;;)
    {
        unsigned bit = (unsigned)__builtin_parityll((unsigned long long)i);
        buf[bufSize++] = (unsigned char)bit;
        ++i;

        if (bufSize == 4)
        {
            unsigned out;
            if (buf[0] == 1 && buf[1] == 1 && buf[2] == 0 && buf[3] == 1)
            {
                out = 0;
                bufSize = 0;
            }
            else
            {
                out = buf[0];
                buf[0] = buf[1];
                buf[1] = buf[2];
                buf[2] = buf[3];
                bufSize = 3;
            }
            ++outIdx;
            if (out == 1)
            {
                ++ones;
                if (ones == n) return (seq_t)outIdx;
            }
        }
    }
}
