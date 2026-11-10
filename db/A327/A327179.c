/*
 * A327179  Positions of 1's in {A327177(n) : n > 0}.
 * https://oeis.org/A327179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th k>=1 with A327177(k)=1, where
 *            A327177(k) = floor((2k+1)*sqrt(2)) - floor((k+1)*sqrt(2)) -
 *            floor(k*sqrt(2)); computed by a linear scan with incremental
 *            integer floor tracking
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9346 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t fk = 0;   /* floor(k*sqrt2) */
    seq_t fk1 = 1;  /* floor((k+1)*sqrt2) */
    seq_t g = 1;    /* floor((2k+1)*sqrt2) */
    idx_t count = 0;
    idx_t k = 0;
    for (;;)
    {
        if (k >= 1)
        {
            seq_t val = g - fk1 - fk;
            if (val == 1)
            {
                count++;
                if (count == n)
                    return (seq_t)k;
            }
        }
        /* advance */
        seq_t m = (seq_t)k + 2;
        seq_t t = fk1 + 2;
        fk = fk1;
        if (t * t <= (seq_t)2 * m * m)
            fk1 = fk1 + 2;
        else
            fk1 = fk1 + 1;
        seq_t m2 = (seq_t)2 * (seq_t)k + 3;
        seq_t t2 = g + 3;
        if (t2 * t2 <= (seq_t)2 * m2 * m2)
            g = g + 3;
        else
            g = g + 2;
        k++;
    }
}
