/*
 * A355633  a(n) is the sum of the divisors of n whose binary expansions
 *          appear as substrings in the binary expansion of n.
 * https://oeis.org/A355633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of all divisors d of n such that bin(d) is a
 *            substring of bin(n)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static void to_bin(uint64_t x, char *buf)
{
    int msb = 0;
    for (int i = 63; i >= 0; --i)
    {
        if ((x >> i) & 1)
        {
            msb = i;
            break;
        }
    }
    size_t pos = 0;
    for (int i = msb; i >= 0; --i)
        buf[pos++] = ((x >> i) & 1) ? '1' : '0';
    buf[pos] = '\0';
}

seq_t a(idx_t n)
{
    /* Build binary representation of n once */
    char binN[65];
    to_bin((uint64_t)n, binN);

    /* Factor n */
    uint64_t primes[16], exps[16];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* Generate all divisors */
    uint64_t divs[512];
    size_t dcnt = 1;
    divs[0] = 1ULL;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        size_t current = dcnt;
        uint64_t p = primes[i];
        uint64_t p_pow = 1ULL;
        for (uint64_t e = 1; e <= exps[i]; ++e)
        {
            p_pow *= p;
            for (size_t j = 0; j < current; ++j)
            {
                divs[dcnt++] = divs[j] * p_pow;
            }
        }
    }

    /* Sum qualifying divisors */
    seq_t sum = (seq_t)0;
    char binD[65];
    for (size_t i = 0; i < dcnt; ++i)
    {
        uint64_t d = divs[i];
        to_bin(d, binD);
        if (strstr(binN, binD) != NULL)
            sum += (seq_t)d;
    }

    return sum;
}
