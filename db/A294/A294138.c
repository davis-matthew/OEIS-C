/*
 * A294138  Number of compositions (ordered partitions) of n into proper
 *          divisors of n.
 * https://oeis.org/A294138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered compositions of n into its proper
 *            divisors For n = 0, a(0) = 1. a(n) for n > 0 is computed with
 *            dynamic programming: let D(n) = { d | d divides n, 1 ≤ d < n }
 *            dp[0] = 1 for s = 1 .. n dp[s] = Σ_{d ∈ D(n), d ≤ s} dp[s-d]
 *            a(n) = dp[n] This counts all ordered sums of element...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 71, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility: factor n into primes p[i]^e[i].
   primes and exponents are written to arrays of length up to 64.
   *count is the number of distinct prime factors. */
static void factor_uint64(uint64_t n, uint64_t primes[64], uint8_t exps[64], int *count)
{
    int c = 0;
    uint64_t temp = n;
    if (temp % 2 == 0) {
        uint8_t e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            e++;
        }
        primes[c] = 2;
        exps[c]   = e;
        c++;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes[c] = p;
            exps[c]   = e;
            c++;
        }
    }
    if (temp > 1) {
        primes[c] = temp;
        exps[c]   = 1;
        c++;
    }
    *count = c;
}

/* Recursively generate all proper divisors of n from its factorization.
   Arguments:
        idx    : current prime factor index
        cur    : product of chosen powers so far
   It stores each divisor < n into array 'divs' and increments '*cnt'.
   The arrays primes[.] and exps[.] contain the factorization. */
static void generate_divisors(uint64_t n, int idx, uint64_t cur,
                              uint64_t primes[64], uint8_t exps[64],
                              int factor_cnt, uint64_t *divs, int *cnt)
{
    if (idx == factor_cnt) {
        if (cur < n) {
            divs[*cnt] = cur;
            (*cnt)++;
        }
        return;
    }
    uint64_t p = primes[idx];
    uint8_t e  = exps[idx];
    uint64_t val = 1;
    for (uint8_t k = 0; k <= e; ++k) {
        generate_divisors(n, idx + 1, cur * val,
                          primes, exps, factor_cnt,
                          divs, cnt);
        val *= p;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;

    /* 1.  Compute proper divisors of n. */
    uint64_t primes[64];
    uint8_t  exps[64];
    int factor_cnt;
    factor_uint64((uint64_t)n, primes, exps, &factor_cnt);

    /* Estimate upper bound of divisor count: 2^(something). For n <= 10000 this is safe. */
    uint64_t divs[256];
    int div_cnt = 0;
    generate_divisors((uint64_t)n, 0, 1,
                      primes, exps, factor_cnt,
                      divs, &div_cnt);

    /* 2.  Dynamic programming for ordered compositions. */
    /* Allocate dp array of size n+1. */
    idx_t size = n + 1;
    seq_t *dp = (seq_t *)malloc((size_t)size * sizeof(seq_t));
    if (!dp) return 0; /* Allocation failure, return 0 as fallback. */

    dp[0] = 1;
    for (idx_t s = 1; s <= n; ++s) {
        seq_t sum = 0;
        for (int i = 0; i < div_cnt; ++i) {
            uint64_t d = divs[i];
            if (d <= (uint64_t)s) {
                idx_t idx = s - d;
                sum += dp[idx];
            }
        }
        dp[s] = sum;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
