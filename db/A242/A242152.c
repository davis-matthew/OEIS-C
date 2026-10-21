/*
 * A242152  Numbers n such that the sum of their unitary prime divisors
 *          divides sigma(n).
 * https://oeis.org/A242152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 487, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A242152: numbers n whose sum of unitary prime divisors
   divides σ(n).  The n‑th term is returned by the function a. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static inline seq_t pow_seq(seq_t base, int exp)
{
    seq_t res = 1;
    for (int i = 0; i < exp; ++i) res *= base;
    return res;
}

static bool satisfies(uint64_t m)
{
    if (m < 2) return false;          /* no prime factors */

    uint64_t temp = m;
    uint64_t unitary_sum = 0;         /* sum of unitary prime divisors */
    seq_t sigma = 1;                  /* σ(m) */

    /* trial division up to sqrt(temp) */
    uint64_t p = 2;
    while (p * p <= temp)
    {
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            if (exp == 1) unitary_sum += p;

            /* σ(p^exp) = (p^(exp+1)-1)/(p-1) */
            seq_t pow_val = pow_seq((seq_t)p, exp + 1);
            seq_t term = (pow_val - 1) / ((seq_t)p - 1);
            sigma *= term;
        }
        p = (p == 2) ? 3 : p + 2;    /* skip even numbers after 2 */
    }

    /* remaining prime factor > sqrt(temp) */
    if (temp > 1)
    {
        unitary_sum += temp;
        /* σ(p) = p + 1 for exponent 1 */
        sigma *= ((seq_t)temp + 1);
    }

    if (unitary_sum == 0) return false;   /* avoid division by 0 */

    return ((sigma % (seq_t)unitary_sum) == 0);
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t m = 1;
    while (true)
    {
        if (satisfies(m))
        {
            ++found;
            if (found == n) return (seq_t)m;
        }
        ++m;
    }
}
