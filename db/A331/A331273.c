/*
 * A331273  Sum of the iterated exponential totient function (A072911).
 * https://oeis.org/A331273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A331273: a(n) = ephi(n) + ephi(ephi(n)) + ... until 0
   where ephi is the multiplicative function ephi(p^k) = φ(k), with ephi(1)=0. */

#include <stdint.h>
#include <stdbool.h>

/* Euler's totient for a small integer m (m ≥ 1). */
static seq_t phi_small(uint64_t m)
{
    seq_t result = (seq_t)m;
    uint64_t tmp = m;

    /* trial division – m is at most the exponent of a prime factor,
       so it is very small (≤ 64 for 64‑bit n). */
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            while (tmp % p == 0)
                tmp /= p;
            result = result / p * (p - 1);
        }
    }
    if (tmp > 1)
        result = result / tmp * (tmp - 1);

    return result;
}

/* Exponential totient for a 64‑bit unsigned integer n. */
static seq_t ephi_uint64(uint64_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    uint64_t rem = n;

    for (uint64_t p = 2; p * p <= rem; ++p)
    {
        if (rem % p == 0)
        {
            uint64_t exp = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++exp;
            }
            if (exp > 1)                   /* φ(1) = 1 – no effect */
                result *= phi_small(exp);
        }
    }

    /* rem now is either 1 or a prime factor with exponent 1 – φ(1)=1, so nothing to do. */
    return result;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t cur = ephi_uint64((uint64_t)n);

    while (cur > (seq_t)0)
    {
        sum += cur;
        cur = ephi_uint64((uint64_t)cur);
    }
    return sum;
}
