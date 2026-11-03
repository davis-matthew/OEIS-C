/*
 * A330225  Position of first appearance of n in A290103 = LCM of prime
 *          indices.
 * https://oeis.org/A330225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the first appearance of n in A290103, i.e.
 *            the smallest m such that the least common multiple of the
 *            indices of the prime factors of m equals n
 * Range:     u8 n <= 8, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper: factorize a small integer using trial division. */
static void factor_idx(idx_t n, idx_t *primes, int *exps, int *cnt) {
    int c = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (m > 1) {
        primes[c] = m;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;

    /* Factor n to obtain the required prime powers. */
    idx_t primes_n[16];
    int exp_n[16];
    int cnt_n = 0;
    factor_idx(n, primes_n, exp_n, &cnt_n);

    unsigned full_mask = (1U << cnt_n) - 1U;

    /* DP over subset of prime powers required. */
    /* Since n <= 10000, cnt_n <= 5, so allocate enough space. */
    seq_t dp[128] = {0};
    dp[0] = 1;

    /* Enumerate all divisors of n except 1. */
    idx_t divs[256];
    int div_cnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (d != 1) divs[div_cnt++] = d;
            idx_t d2 = n / d;
            if (d2 != d && d2 != 1) divs[div_cnt++] = d2;
        }
    }

    /* Process each divisor: determine which prime powers it covers. */
    for (int di = 0; di < div_cnt; ++di) {
        idx_t i = divs[di];
        unsigned mask = 0;
        for (int j = 0; j < cnt_n; ++j) {
            int e = 0;
            idx_t tmp = i;
            while (tmp % primes_n[j] == 0) {
                tmp /= primes_n[j];
                ++e;
            }
            if (e >= exp_n[j]) mask |= (1U << j);
        }
        if (mask == 0) continue;                /* nothing to cover */

        seq_t prime_i = get_prime(i);

        /* 0‑1 knapsack style DP. */
        for (unsigned m = 0; m <= full_mask; ++m) {
            if (dp[m] == 0) continue;
            unsigned nm = m | mask;
            seq_t val = dp[m] * prime_i;
            if (dp[nm] == 0 || val < dp[nm]) dp[nm] = val;
        }
    }

    return dp[full_mask];
}
