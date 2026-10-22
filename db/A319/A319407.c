/*
 * A319407  a(n) = A175046(n)-n.
 * https://oeis.org/A319407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A175046(n) - n
 * Range:     u8 n <= 17, u16 n <= 297, u32 n <= 9844, u64 n <= 9844,
 *            u128 n <= 9844, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175046(n) is obtained by extending every run of equal bits in the binary
   representation of n by one additional bit of the same value. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 0;
    int current_bit = -1;    /* last bit seen in the input run */
    int run_len = 0;         /* length of the current run */

    /* Determine the number of bits in n (n >= 1) */
    idx_t temp = n;
    idx_t len = 0;
    while (temp)
    {
        ++len;
        temp >>= 1;
    }

    /* Scan bits from most significant to least significant */
    for (int i = (int)len - 1; i >= 0; --i)
    {
        int bit = (n >> i) & 1;
        if (bit == current_bit)
        {
            ++run_len;
        }
        else
        {
            if (current_bit != -1)
            {
                /* Output the previous run extended by one bit */
                for (int k = 0; k < run_len + 1; ++k)
                    r = (r << 1) | (seq_t)current_bit;
            }
            current_bit = bit;
            run_len = 1;
        }
    }

    /* Output the last run */
    if (current_bit != -1)
    {
        for (int k = 0; k < run_len + 1; ++k)
            r = (r << 1) | (seq_t)current_bit;
    }

    return r - (seq_t)n;
}
