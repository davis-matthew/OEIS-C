/*
 * A347464  Number of even-length ordered factorizations of n^2 into factors >
 *          1 with alternating product 1.
 * https://oeis.org/A347464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} f_k(n)^2 where f_k(n) is the number of ordered
 *            factorizations of n into exactly k integers >1
 * Range:     u8 n <= 47, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a recursion with memoisation.
   The recursion depth is at most floor(log₂(n)) <= 13 for n ≤ 10⁴.
   The cache size is 10001×15 entries; for 128‑bit seq_t this is
   about 2 MB, well inside typical limits. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXN 10000   /* maximum n that can be queried */
#define MAXK 14      /* floor(log₂(MAXN)) + 1 = 13 + 1 */

/* global memoisation tables: memo[n][k] holds f_k(n). #pragma for visibility */
static seq_t memo[MAXN + 1][MAXK + 1];
static bool   memo_computed[MAXN + 1][MAXK + 1];

/* helper: trial division factorisation of n (n ≤ MAXN). */
static void factor_uint64(uint64_t m, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t len = 0;
    if (m < 2) { *cnt = 0; return; }

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pr[len] = p;
            ex[len] = e;
            ++len;
        }
    }
    if (m > 1) { pr[len] = m; ex[len] = 1; ++len; }

    *cnt = len;
}

/* helper: generate all divisors of a number given its prime factorisation.
   The result in 'divs' excludes 1.  The returned count is in 'cnt_out'. */
static void gen_divs(const uint64_t *pr,
                     const uint64_t *ex,
                     size_t          prime_cnt,
                     size_t          idx,
                     uint64_t        cur,
                     uint64_t *      divs,
                     size_t *        cnt_out)
{
    if (idx == prime_cnt) {
        if (cur > 1) { divs[(*cnt_out)++] = cur; }
        return;
    }
    uint64_t next = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divs(pr, ex, prime_cnt, idx + 1, next, divs, cnt_out);
        next *= pr[idx];
    }
}

/* recursive computation of f_k(n).  Uses memoisation. */
static seq_t f_k_uint64(uint64_t n, uint64_t k)
{
    if (k == 1) return (n > 1 ? (seq_t)1 : (seq_t)0);
    if (n == 1) return (seq_t)0;

    if (n <= (uint64_t)MAXN && k <= (uint64_t)MAXK) {
        if (memo_computed[n][k]) return memo[n][k];
    }

    /* factor n once */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t   pcnt;
    factor_uint64(n, primes, exps, &pcnt);

    /* generate all divisors >1 of n */
    uint64_t divs[200];
    size_t   dcnt = 0;
    gen_divs(primes, exps, pcnt, 0, 1, divs, &dcnt);

    seq_t total = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d <= 1) continue;          /* safety */
        uint64_t nd = n / d;
        seq_t sub = f_k_uint64(nd, k - 1);
        total += sub;
    }

    if (n <= (uint64_t)MAXN && k <= (uint64_t)MAXK) {
        memo[n][k] = total;
        memo_computed[n][k] = true;
    }
    return total;
}

/* public entry point required by the framework */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    /* compute floor(log₂(n)) as maximum possible factorisation length */
    idx_t maxk = 0;
    idx_t ph   = 1;
    while ((ph << 1) <= n) { ph <<= 1; ++maxk; }

    seq_t sum = 0;
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t f = f_k_uint64((uint64_t)n, (uint64_t)k);
        sum += f * f;
    }
    return sum;
}
