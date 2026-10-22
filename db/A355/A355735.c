/*
 * A355735  Number of ways to choose a divisor of each prime index of n (taken
 *          in weakly increasing order) such that the result is weakly
 *          increasing.
 * https://oeis.org/A355735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weakly increasing sequences of divisors of the
 *            prime‑indices of n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static idx_t   max_prime_limit = 0;      /* primes built up to this value   */
static uint64_t *primes = NULL;          /* array of primes                  */
static size_t  prime_cnt = 0;            /* number of primes found           */

static idx_t   max_div_index = 0;        /* largest index for which tables
                                            are built (equal to prime_cnt)  */
static idx_t **idx_divisors = NULL;      /* idx_divisors[i] = sorted array of
                                            divisors of i (i ≥ 1)             */
static size_t *idx_div_cnt   = NULL;      /* number of divisors of i           */

/* Compare two idx_t values for qsort. */
static int cmp_idx(const void *a, const void *b)
{
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Free a divisor table. */
static void free_divisors(void)
{
    if (idx_divisors) {
        for (idx_t i = 1; i <= max_div_index; ++i)
            free(idx_divisors[i]);
        free(idx_divisors);
        idx_divisors = NULL;
    }
    if (idx_div_cnt) {
        free(idx_div_cnt);
        idx_div_cnt = NULL;
    }
}

/* Rebuild primes and divisor tables up to new_limit. */
static void rebuild(idx_t new_limit)
{
    /* free old data */
    free(primes); primes = NULL;
    prime_cnt = 0;
    free_divisors();
    max_div_index = 0;

    /* Build new prime list up to new_limit (at least 2). */
    idx_t limit = new_limit < 2 ? 2 : new_limit;
    build_primes_upto(limit, &primes, &prime_cnt);
    max_prime_limit = limit;

    /* Allocate divisor tables for indices 1 .. prime_cnt. */
    max_div_index = (idx_t)prime_cnt;
    idx_divisors = (idx_t **)calloc(max_div_index + 1, sizeof(idx_t *));
    idx_div_cnt   = (size_t *)calloc(max_div_index + 1, sizeof(size_t));

    for (idx_t m = 1; m <= max_div_index; ++m) {
        /* count divisors */
        size_t cnt = 0;
        for (idx_t d = 1; d * d <= m; ++d)
            if (m % d == 0) {
                ++cnt;
                if (d != m / d) ++cnt;
            }

        idx_t *arr = (idx_t *)malloc(cnt * sizeof(idx_t));
        size_t pos = 0;
        for (idx_t d = 1; d * d <= m; ++d)
            if (m % d == 0) {
                arr[pos++] = d;
                if (d != m / d) arr[pos++] = m / d;
            }

        qsort(arr, cnt, sizeof(idx_t), cmp_idx);
        idx_divisors[m] = arr;
        idx_div_cnt[m]  = cnt;
    }
}

/* Binary search in the primes array to return 1‑based index of p. */
static idx_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)(lo + 1);   /* 1‑based */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;      /* special case: no prime factors */

    /* Ensure tables are built up to n (primes up to n). */
    if (primes == NULL || n > max_prime_limit)
        rebuild(n);

    /* Factor n and collect prime indices (with multiplicity), sorted. */
    uint64_t tmp = (uint64_t)n;
    idx_t *idxs = (idx_t *)calloc(8, sizeof(idx_t));
    size_t cap = 8, len = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > tmp / p) break;           /* p > sqrt(tmp) */
        if (tmp % p != 0) continue;
        size_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        idx_t id = prime_index(p);
        for (size_t e = 0; e < exp; ++e) {
            if (len == cap) {
                cap *= 2;
                idxs = (idx_t *)realloc(idxs, cap * sizeof(idx_t));
            }
            idxs[len++] = id;
        }
    }
    if (tmp > 1) {               /* remaining prime > sqrt(original n) */
        idx_t id = prime_index(tmp);
        if (len == cap) {
            cap *= 2;
            idxs = (idx_t *)realloc(idxs, cap * sizeof(idx_t));
        }
        idxs[len++] = id;
    }

    /* Dynamic programming over sorted indices. */
    seq_t *dp_prev = NULL;
    const idx_t *prev_divs = NULL;
    size_t dp_prev_cnt = 0;

    /* Initialize with first index. */
    idx_t first = idxs[0];
    dp_prev_cnt = idx_div_cnt[first];
    prev_divs   = idx_divisors[first];
    dp_prev = (seq_t *)calloc(dp_prev_cnt, sizeof(seq_t));
    for (size_t i = 0; i < dp_prev_cnt; ++i) dp_prev[i] = (seq_t)1;

    /* Successive indices. */
    for (size_t pos = 1; pos < len; ++pos) {
        idx_t cur = idxs[pos];
        const idx_t *cur_divs = idx_divisors[cur];
        size_t cur_cnt = idx_div_cnt[cur];

        seq_t *dp_cur = (seq_t *)calloc(cur_cnt, sizeof(seq_t));

        size_t p = 0;
        seq_t pref = 0;
        for (size_t i = 0; i < cur_cnt; ++i) {
            idx_t d = cur_divs[i];
            while (p < dp_prev_cnt && prev_divs[p] <= d) {
                pref = pref + dp_prev[p];
                ++p;
            }
            dp_cur[i] = pref;
        }

        free(dp_prev);
        dp_prev = dp_cur;
        prev_divs = cur_divs;
        dp_prev_cnt = cur_cnt;
    }

    /* Sum final counts. */
    seq_t ans = 0;
    for (size_t i = 0; i < dp_prev_cnt; ++i) ans += dp_prev[i];

    /* Clean up. */
    free(dp_prev);
    free(idxs);

    return ans;
}
