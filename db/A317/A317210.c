/*
 * A317210  Composite numbers k+1 such that A002322(A027760(k)) = k.
 * https://oeis.org/A317210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 9759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Composite numbers n such that
   LCM{ d | d divides n-1 and d+1 is prime } = n-1.
   The first 24 terms are 21, 45, 49, 81, 85, 91, 93, 111, 117, 121, ... */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* LCM for seq_t values. */
static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

/* Recursive enumeration of all divisors of m = pr[0]^ex[0] * ... * pr[cnt-1]^ex[cnt-1].
   For each divisor d, if d+1 is prime, include d in the accumulated LCM. */
static void rec_divs(size_t pos, uint64_t cur, const uint64_t *pr,
                     const uint64_t *ex, size_t cnt, seq_t *lcm)
{
    if (pos == cnt) {
        if (is_prime(cur + 1)) {
            seq_t d = (seq_t)cur;
            *lcm = lcm_seq(*lcm, d);
        }
        return;
    }

    uint64_t p = pr[pos];
    uint64_t val = cur;
    for (size_t e = 0; e <= ex[pos]; ++e) {
        rec_divs(pos + 1, val, pr, ex, cnt, lcm);
        if (e < ex[pos]) {
            val *= p;
        }
    }
}

/* Compute the nth term of A317210 (offset 1). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* Current composite candidate to test. */
    static idx_t candidate = 4;

    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Find the next composite n that satisfies the LCM condition. */
        while (true) {
            if (is_prime(candidate)) {
                ++candidate;
                continue;
            }

            uint64_t m = (uint64_t)(candidate - 1);
            uint64_t primes[32];
            uint64_t exps[32];
            size_t pcnt = 0;

            factor_u64(m, primes, exps, &pcnt);

            seq_t lcm_val = (seq_t)1;
            rec_divs(0, 1, primes, exps, pcnt, &lcm_val);

            if (lcm_val == (seq_t)m) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)candidate;
                ++candidate;
                break;
            }

            ++candidate;
        }
    }

    return cache[n - 1];
}
