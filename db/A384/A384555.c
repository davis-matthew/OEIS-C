/*
 * A384555  The largest infinitary divisor of n that is cubefree.
 * https://oeis.org/A384555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e | n} f(e) * where f(e) = 1 if e ≡ 0 (mod 4) * p if
 *            e ≡ 1 (mod 4) * p² if e ≡ 2 or 3 (mod 4)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t value = (uint64_t)n;

    for (uint64_t p = 2; p * p <= value; ++p)
    {
        if (value % p == 0)
        {
            uint64_t e = 0;
            while (value % p == 0)
            {
                value /= p;
                ++e;
            }
            uint64_t mod4 = e & 3ULL;          /* e mod 4 */
            seq_t factor;
            if (mod4 == 1)
                factor = (seq_t)p;           /* p¹ */
            else if (mod4 == 2 || mod4 == 3)
                factor = (seq_t)p * (seq_t)p;/* p² */
            else
                factor = (seq_t)1;           /* e ≡ 0 (mod 4) */
            result *= factor;
        }
    }

    /* Remaining prime factor (exponent 1) */
    if (value > 1)
    {
        seq_t factor = (seq_t)value;          /* p¹ */
        result *= factor;
    }

    return result;
}
