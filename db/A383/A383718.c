/*
 * A383718  a(n) is the multinomial coefficient (length of n in binary) choose
 *          (the lengths of runs in n's binary expansion).
 * https://oeis.org/A383718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = multinomial coefficient of the binary length of n * and
 *            the lengths of its runs of consecutive bits
 * Range:     u8 n <= 36, u16 n <= 292, u32 n <= 5460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8031 (2026-11-01)
 * Helpers:   ilog2, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ilog2.h"
#include "../utils/ipow_u64_idx.h"

static inline idx_t count_p_factorial(idx_t n, unsigned int p)
{
    idx_t e = 0;
    while (n)
    {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* number of bits in n */
    idx_t len = ilog2(n) + 1;

    /* collect run lengths */
    unsigned int runs[64];
    size_t run_count = 0;

    idx_t pos = len - 1;
    unsigned int current = (n >> pos) & 1;
    unsigned int cnt = 1;
    while (pos > 0)
    {
        --pos;
        unsigned int bit = (n >> pos) & 1;
        if (bit == current)
            ++cnt;
        else
        {
            runs[run_count++] = cnt;
            current = bit;
            cnt = 1;
        }
    }
    runs[run_count++] = cnt;

/* primes needed for factorisation up to 64 */
    static const unsigned int primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59
    };
    const size_t primes_cnt = sizeof(primes) / sizeof(primes[0]);

    int exp_diff[primes_cnt];
    for (size_t i = 0; i < primes_cnt; ++i)
        exp_diff[i] = 0;

    /* add exponents from len! */
    for (size_t i = 0; i < primes_cnt; ++i)
        exp_diff[i] += (int)count_p_factorial(len, primes[i]);

    /* subtract exponents for each run factorial */
    for (size_t r = 0; r < run_count; ++r)
    {
        idx_t rr = runs[r];
        for (size_t i = 0; i < primes_cnt; ++i)
            exp_diff[i] -= (int)count_p_factorial(rr, primes[i]);
    }

    /* compute the product of p^exp_diff */
    seq_t result = 1;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        if (exp_diff[i] > 0)
            result *= ipow_u64_idx(primes[i], (idx_t)exp_diff[i]);
    }
    return result;
}
