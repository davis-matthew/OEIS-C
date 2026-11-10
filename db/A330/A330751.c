/*
 * A330751  Number of values of k, 1 <= k <= n, with A319626(k) = A319626(n),
 *          where A319626(n) gives the numerator of rational valued primorial
 *          deflation of n.
 * https://oeis.org/A330751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : 1 <= k <= n, A319626(k) = A319626(n)}, computed by
 *            direct counting (ordinal transform of A319626)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9903 (2026-11-08)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static uint64_t A319626_val(uint64_t n)
{
    if (n <= 1)
        return 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t pr = primes[i];
        uint64_t exp_n = exps[i];
        uint64_t next_pr = next_prime(pr);
        uint64_t exp_next = 0;
        for (size_t j = 0; j < cnt; ++j)
        {
            if (primes[j] == next_pr)
            {
                exp_next = exps[j];
                break;
            }
        }
        uint64_t g = exp_n < exp_next ? exp_n : exp_next;
        uint64_t e = exp_n - g;
        while (e--)
            result *= pr;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    uint64_t target = A319626_val((uint64_t)n);
    uint64_t count = 0;
    for (uint64_t k = 1; k <= (uint64_t)n; ++k)
    {
        if (A319626_val(k) == target)
            ++count;
    }
    return (seq_t)count;
}
