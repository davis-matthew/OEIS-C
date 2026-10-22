/*
 * A359147  Partial sums of A002326.
 * https://oeis.org/A359147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A007733(2*k+1)
 * Range:     u8 n <= 20, u16 n <= 437, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Modular exponentiation: (base^exp) mod mod.
   All intermediate arithmetic fits in 64 bit because arguments are
   < 20001 for the problem limits. */
static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* Euler's totient φ(n) for 64–bit unsigned n.  Small n (< 20001)
   so simple trial division is adequate. */
static uint64_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t result = n;
    uint64_t m = n;
    if ((m & 1ULL) == 0)
    {
        /* factor 2 separately */
        while ((m & 1ULL) == 0) m >>= 1;
        result = result / 2 * 1;
    }
    for (uint64_t p = 3; p * p <= m; p += 2)
    {
        if (m % p == 0)
        {
            while (m % p == 0) m /= p;
            result = result / p * (p - 1);
        }
    }
    if (m > 1)  /* remaining prime factor */
        result = result / m * (m - 1);
    return result;
}

/* Compute the multiplicative order of 2 modulo odd m (m≥1). */
static uint64_t order_of_two(uint64_t m)
{
    if (m == 1ULL) return 1ULL;          /* by convention */

    uint64_t phi = phi_uint64(m);

    /* Distinct prime factors of φ(m) – small numbers, so array up to 64. */
    uint64_t distinct[64];
    uint64_t cnt = 0;
    uint64_t t = phi;
    if (t % 2ULL == 0)
    {
        distinct[cnt++] = 2ULL;
        while (t % 2ULL == 0) t /= 2ULL;
    }
    for (uint64_t p = 3; p * p <= t; p += 2)
    {
        if (t % p == 0)
        {
            distinct[cnt++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1ULL) distinct[cnt++] = t;

    uint64_t ord = phi;
    for (uint64_t i = 0; i < cnt; ++i)
    {
        uint64_t prime = distinct[i];
        while (ord % prime == 0)
        {
            uint64_t cand = ord / prime;
            if (pow_mod_uint64(2ULL, cand, m) == 1ULL)
                ord = cand;
            else
                break;
        }
    }
    return ord;
}

/* External linkage function requested by the build system. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        uint64_t m = 2ULL * (uint64_t)k + 1ULL;   /* odd m */
        uint64_t ord = order_of_two(m);
        sum += (seq_t)ord;
    }
    return sum;
}
