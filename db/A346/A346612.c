/*
 * A346612  Moebius transform of A019554.
 * https://oeis.org/A346612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n, e odd} p^{(e−1)/2}·(p−1), a(n) = 0 if any
 *            exponent e is even. a(1) = 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   powseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/powseq.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p != 0)
            continue;

        unsigned e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }

        if ((e & 1) == 0)               /* even exponent → zero */
            return (seq_t)0;

        unsigned k = (e - 1) >> 1;      /* (e−1)/2 */
        seq_t factor = powseq((seq_t)p, k);
        factor *= ((seq_t)p - 1);
        result *= factor;
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        seq_t factor = (seq_t)temp - 1; /* exponent e = 1 ⇒ k = 0 */
        result *= factor;
    }

    return result;
}
