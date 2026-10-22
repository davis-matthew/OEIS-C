/*
 * A353645  a(1) = 1, and for n > 1, number of ways to write the square of n
 *          as a product of at least two factors, the largest two of which are
 *          equal.
 * https://oeis.org/A353645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n^2 as a product of at least *
 *            two factors, where the two largest factors are equal. This is *
 *            A325045(A000290(n)). The implementation enumerates all
 *            multiplicative * partitions of n^2 in non‑decreasing order and
 *            counts those whose * largest factor occurs at least t...
 * Range:     u8 n <= 575, u16 n <= 9724, u32 n <= 9724, u64 n <= 9724,
 *            u128 n <= 9724, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

/* comparison function for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) - (x > y);
}

/* static divisor array */
static uint64_t *divs = NULL;
static size_t  div_cnt = 0;
static size_t  div_cap = 0;

/* ensure divisor array capacity */
static void ensure_divs_cap(size_t needed)
{
    if (needed > div_cap) {
        size_t new_cap = div_cap ? div_cap * 2 : 64;
        while (new_cap < needed) new_cap *= 2;
        divs = (uint64_t *)realloc(divs, new_cap * sizeof(uint64_t));
        div_cap = new_cap;
    }
}

/* recursive generation of divisors from prime factors */
static void gen_divs_rec(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         size_t idx, uint64_t cur)
{
    if (idx == cnt) {
        ensure_divs_cap(div_cnt + 1);
        divs[div_cnt++] = cur;
        return;
    }
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divs_rec(pr, ex, cnt, idx + 1, val);
        val = val * pr[idx];
    }
}

/* recursive counting of factorizations with the largest two equal */
static seq_t count_factorizations(uint64_t rem,
                                 uint64_t last_fact,
                                 uint64_t max_fact,
                                 uint64_t max_cnt,
                                 uint64_t depth)
{
    if (rem == 1) {
        return (depth >= 2 && max_cnt >= 2) ? (seq_t)1 : (seq_t)0;
    }

    seq_t res = 0;

    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        if (d < last_fact || d < 2) continue;          /* skip 1 and decreasing */
        if (rem % d != 0) continue;

        uint64_t new_max_fact = max_fact;
        uint64_t new_max_cnt  = max_cnt;

        if (d > max_fact) {            /* new maximum factor seen */
            new_max_fact = d;
            new_max_cnt  = 1;
        } else if (d == max_fact) {    /* duplicate the maximum */
            new_max_cnt++;
        }

        res += count_factorizations(rem / d, d, new_max_fact, new_max_cnt, depth + 1);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition of the sequence */
    if (n == 1) return (seq_t)1;

    /* compute m = n^2 */
    uint64_t n64 = (uint64_t)n;
    uint64_t m   = n64 * n64;    /* fits in 64 bits for n <= 4e9 */

    /* factor m: primes and exponents doubled from n's factorization */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t   pcnt = 0;
    factor_u64(m, primes, exps, &pcnt);

    /* generate all divisors of m */
    div_cnt = 0;
    gen_divs_rec(primes, exps, pcnt, 0, 1ULL);

    /* sort divisor list for efficient non‑decreasing search */
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* count factorizations with the required property */
    seq_t result = count_factorizations(m, 2, 0, 0, 0);
    return result;
}
