/*
 * A327939  Multiplicative with a(p^e) = p^(e-(e mod p)).
 * https://oeis.org/A327939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of p^(e - (e mod
 *            p)) (Multiplicative with a(p^e) = p^(e-(e mod p)))
 * Range:     u8 n <= 255, u16 n <= 9573, u32 n <= 9573, u64 n <= 9573,
 *            u128 n <= 9573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* fast exponentiation for integer base and exponent (exponent fits in unsigned) */
static seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out‑of‑range values are undefined, return 0 */

    seq_t res = 1;
    uint64_t temp = n;
    unsigned e = 0;

    /* handle prime 2 separately */
    while ((temp & 1ULL) == 0)
    {
        e++;
        temp >>= 1;
    }
    if (e > 0)
    {
        unsigned e_minus = e - (e % 2);
        if (e_minus > 0)
            res *= pow_seq((seq_t)2, e_minus);
    }

    /* odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p != 0) continue;
        e = 0;
        while (temp % p == 0)
        {
            e++;
            temp /= p;
        }
        unsigned e_minus = e - (e % p);
        if (e_minus > 0)
            res *= pow_seq((seq_t)p, e_minus);
    }

    /* remaining prime factor (if any) */
    if (temp > 1)
    {
        /* e = 1 for the remaining prime factor */
        /* e_minus = e - (e mod temp) = 1 - (1 mod temp) = 0 (unless temp == 1) */
        /* so it contributes nothing; the product remains unchanged */
    }

    return res;
}
