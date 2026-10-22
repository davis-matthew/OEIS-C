/*
 * A327502  a(n) = n/A327501(n), where A327501(n) is the maximum divisor of n
 *          that is 1 or not a perfect power.
 * https://oeis.org/A327502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n / A327501(n) where A327501(n) is the maximum divisor of
 *            n that is 1 or not a perfect power
 * Range:     u8 n <= 511, u16 n <= 9803, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test if x (uint64_t) is a perfect power a^e with a>1, e>1. */
static bool is_perfect_power_uint64(uint64_t x)
{
    if (x < 4)            /* smallest perfect power is 2^2 = 4 */
        return false;
    for (uint64_t base = 2; ; ++base) {
        unsigned __int128 sq = (unsigned __int128) base * base;
        if (sq > x)       /* base^2 > x, stop */
            break;
        unsigned __int128 val = sq;
        if (val == x)
            return true;
        while (val < x) {
            if (val > (unsigned __int128) x / base)
                break;   /* next multiplication would overflow x */
            val *= base;
            if (val == x)
                return true;
        }
    }
    return false;
}

/* Recursively enumerate divisor products from prime factorization. */
static void gen_divisors(int idx, seq_t curr, seq_t n,
                         const seq_t *prime, const int *exp,
                         int total, seq_t *dmax)
{
    if (idx == total) {
        if (curr > 1 && curr < n && !is_perfect_power_uint64((uint64_t) curr))
            if (curr > *dmax) *dmax = curr;
        return;
    }
    seq_t p = prime[idx];
    seq_t val = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divisors(idx + 1, curr * val, n, prime, exp, total, dmax);
        val *= p;
    }
}

/* Computes a(n) for the given index n (1‑based). */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t) n;

    /* For a non‑perfect‑power n, the divisor A327501(n) is n itself. */
    if (!is_perfect_power_uint64((uint64_t) n))
        return (seq_t) 1;

    /* Factor n (uint64_t) into primes and exponents. */
    seq_t prime[32];
    int exp[32];
    int tot = 0;
    uint64_t tmp = (uint64_t) n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            prime[tot] = (seq_t) p;
            exp[tot]   = e;
            ++tot;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        prime[tot] = (seq_t) tmp;
        exp[tot]   = 1;
        ++tot;
    }

    /* Find the largest proper divisor of n that is not a perfect power. */
    seq_t dmax = 1;
    gen_divisors(0, 1, nseq, prime, exp, tot, &dmax);

    return nseq / dmax;
}
