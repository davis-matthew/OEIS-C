/*
 * A399440  Number of pairs (d, e) of divisors of n with d < e < 2*d.
 * https://oeis.org/A399440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5039, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts all unordered pairs (d, e) of divisors of n with d < e < 2*d. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Comparator for qsort on uint64_t values. */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Recursive generation of all divisors from prime factorisation. */
static void generate_divisors(const uint64_t *pr, const uint64_t *ex,
                              size_t pf_cnt, size_t idx,
                              uint64_t curr, uint64_t *out,
                              size_t *pos)
{
    if (idx == pf_cnt) {
        out[(*pos)++] = curr;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t mult = 1;
    for (size_t e = 0; e <= ex[idx]; ++e) {
        generate_divisors(pr, ex, pf_cnt, idx + 1, curr * mult, out, pos);
        mult *= p;
    }
}

/* OEIS A399440: number of pairs of divisors of n with ratio in (1,2). */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    /* Factor n. */
    uint64_t primes[64], exps[64];
    size_t pf_cnt = 0;
    factor_u64(n, primes, exps, &pf_cnt);

    /* Compute total number of divisors. */
    size_t divisor_cnt = 1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        divisor_cnt *= (size_t)(exps[i] + 1);
    }

    /* Generate all divisors. */
    uint64_t *divs = (uint64_t *)malloc(divisor_cnt * sizeof(uint64_t));
    if (!divs) return 0;          /* In case of allocation failure */
    size_t sz = 0;
    generate_divisors(primes, exps, pf_cnt, 0, 1, divs, &sz);

    /* Sort the divisors. */
    qsort(divs, sz, sizeof(uint64_t), cmp_uint64);

    /* Count pairs (d, e) with d < e < 2*d. */
    seq_t total = 0;
    size_t j = 1;
    for (size_t i = 0; i < sz; ++i) {
        if (j < i + 1) j = i + 1;
        while (j < sz && (unsigned __int128)divs[j] < (unsigned __int128)2 * divs[i]) {
            ++j;
        }
        total += (seq_t)(j - i - 1);
    }

    free(divs);
    return total;
}
