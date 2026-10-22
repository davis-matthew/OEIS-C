/*
 * A319984  Fully multiplicative with a(p^e) = prime(p mod 4)^e.
 * https://oeis.org/A319984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e of prime(p mod 4)^e where
 *            prime(k) is the k‑th prime (prime(1)=2, prime(2)=3, prime(3)=5)
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    seq_t res = (seq_t)1;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            seq_t q;
            uint64_t r = p % 4;
            if (r == 1) q = (seq_t)2;
            else if (r == 2) q = (seq_t)3;
            else if (r == 3) q = (seq_t)5;
            else continue;          /* should not happen */

            seq_t pk = (seq_t)1;
            for (uint64_t i = 0; i < e; ++i)
                pk *= q;
            res *= pk;
        }
    }

    if (temp > 1)                /* remaining prime factor */
    {
        uint64_t p = temp;
        uint64_t r = p % 4;
        seq_t q;
        if (r == 1) q = (seq_t)2;
        else if (r == 2) q = (seq_t)3;
        else if (r == 3) q = (seq_t)5;
        else q = (seq_t)1;      /* unreachable */
        res *= q;               /* exponent is 1 */
    }

    return res;
}
