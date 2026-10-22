/*
 * A365480  The sum of unitary divisors of the smallest exponentially odd
 *          number that is divisible by n.
 * https://oeis.org/A365480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = σ*( n * ∏_{p^e‖n, e even} p )
 * Range:     u8 n <= 48, u16 n <= 1680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             σ*(m) = ∏_{p^f‖m} (1 + p^f), where f = e + (e even ? 1 : 0) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The answer for all n >= 1 is given by the product over prime factors
       of n: for each prime power p^e | n, let f = e + 1 if e is even else e.
       Then the value contributed by this prime is (1 + p^f).          */
    if (n == 0) return 0;          /* undefined for n=0, but safe guard */

    seq_t result = 1;

    /* Factorization using trial division (efficient enough for n <= 10⁴). */
    idx_t temp = n;
    /* handle factor 2 separately */
    idx_t e = 0;
    while ((temp & 1ULL) == 0)
    {
        temp >>= 1;
        ++e;
    }
    if (e > 0)
    {
        idx_t f = e + ((e & 1ULL) == 0 ? 1 : 0);
        seq_t p_seq = (seq_t)2;
        seq_t pow_val = big_pow(p_seq, (seq_t)f);
        seq_t term = pow_val + (seq_t)1;
        result *= term;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }
        if (e > 0)
        {
            idx_t f = e + ((e & 1ULL) == 0 ? 1 : 0);
            seq_t p_seq = (seq_t)p;
            seq_t pow_val = big_pow(p_seq, (seq_t)f);
            seq_t term = pow_val + (seq_t)1;
            result *= term;
        }
    }

    /* possible remaining prime > sqrt(original n) */
    if (temp > 1)
    {
        /* remaining prime has exponent e = 1 (odd) */
        idx_t f = 1;                /* because e=1 is odd */
        seq_t p_seq = (seq_t)temp;
        seq_t pow_val = big_pow(p_seq, (seq_t)f);  /* p^1 */
        seq_t term = pow_val + (seq_t)1;           /* 1 + p */
        result *= term;
    }

    return result;
}
