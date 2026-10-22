/*
 * A393137  a(n) is the number of nontrivial divisors {d | n, 1 < d < n} that
 *          are sum-irreducible, for n >= 1; by convention a(0) = 0.
 * https://oeis.org/A393137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = number of nontrivial divisors d of n that are sum‑irreducible.
   d is sum‑irreducible if no subset of its other nontrivial divisors sums to d.
   The function enumerates all divisors of n and for each nontrivial divisor performs
   a subset‑sum test using a boolean DP array. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

#define MAX_DIVISORS 2048   /* enough for n ≤ 10 000 and much larger */

static void gen_divisors_rec(size_t idx, uint64_t cur,
                             const uint64_t primes[], const uint64_t exps[],
                             size_t pcnt, uint64_t *list, size_t *cnt)
{
    if (idx == pcnt) {
        list[(*cnt)++] = cur;
        return;
    }
    uint64_t base = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors_rec(idx + 1, cur * base, primes, exps, pcnt, list, cnt);
        base *= primes[idx];
    }
}

static void get_divisors(uint64_t n, uint64_t *list, size_t *cnt)
{
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);
    *cnt = 0;
    gen_divisors_rec(0, (uint64_t)1, primes, exps, pcnt, list, cnt);
}

seq_t a(idx_t n)
{
    if ((uint64_t)n <= 1)
        return (seq_t)0;

    const uint64_t N = (uint64_t)n;
    uint64_t all_div[MAX_DIVISORS];
    size_t all_cnt;
    get_divisors(N, all_div, &all_cnt);

    seq_t result = 0;

    for (size_t i = 0; i < all_cnt; ++i) {
        const uint64_t d = all_div[i];
        if (d <= 1 || d == N)
            continue; /* trivial divisor */

        uint64_t sub_div[MAX_DIVISORS];
        size_t sub_cnt;
        get_divisors(d, sub_div, &sub_cnt);

        /* collect nontrivial divisors of d */
        uint64_t nontriv[MAX_DIVISORS];
        size_t nontriv_cnt = 0;
        for (size_t j = 0; j < sub_cnt; ++j) {
            const uint64_t v = sub_div[j];
            if (v > 1 && v < d)
                nontriv[nontriv_cnt++] = v;
        }

        if (nontriv_cnt == 0) {      /* d is prime (or 1<>d) */
            result++;
            continue;
        }

        /* quick rejection: if sum of all nontrivial divisors < d,
           no subset can reach d */
        uint64_t sum = 0;
        for (size_t k = 0; k < nontriv_cnt; ++k)
            sum += nontriv[k];
        if (sum < d) {
            result++;
            continue;
        }

        /* subset sum DP */
        size_t sz = (size_t)d + 1;
        /* use unsigned char as bool to avoid stdbool overhead */
        unsigned char *dp = (unsigned char *)calloc(sz, sizeof(unsigned char));
        if (!dp)
            return result; /* OOM – return whatever we have computed so far */

        dp[0] = 1;
        bool reached = false;
        for (size_t k = 0; k < nontriv_cnt && !reached; ++k) {
            const uint64_t val = nontriv[k];
            if (val > d)
                continue;
            long v = (long)val;
            for (long s = (long)d; s >= v; --s) {
                if (dp[s - v]) {
                    dp[s] = 1;
                    if (s == (long)d) {
                        reached = true;
                        break;
                    }
                }
            }
        }
        if (!reached)
            result++;

        free(dp);
    }

    return result;
}
