/*
 * A367406  The exponentially odd numbers (A268335) multiplied by their
 *          squarefree kernels (A007947).
 * https://oeis.org/A367406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (the n-th exponentially odd number) × (its squarefree
 *            kernel)
 * Range:     u8 n <= 12, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 1;

    while (1)
    {
        uint64_t temp = candidate;
        seq_t rad = 1;
        int ok = 1;

        for (uint64_t p = 2; p * p <= temp; ++p)
        {
            if (temp % p == 0)
            {
                unsigned int exp = 0;
                while (temp % p == 0)
                {
                    temp /= p;
                    ++exp;
                }
                if ((exp & 1) == 0)   /* even exponent -> not exponentially odd */
                {
                    ok = 0;
                    break;
                }
                rad *= (seq_t)p;      /* add prime only once (square‑free part) */
            }
        }

        if (ok && temp > 1)          /* remaining prime factor has exponent 1 */
            rad *= (seq_t)temp;

        if (ok)
        {
            ++count;
            if (count == n)
            {
                seq_t m = (seq_t)candidate;
                return m * rad;
            }
        }

        ++candidate;
    }
}
