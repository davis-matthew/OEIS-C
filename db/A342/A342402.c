/*
 * A342402  Numbers k with property that if k has m proper divisors, there are
 *          m/2 proper divisors of k whose sum is k.
 * https://oeis.org/A342402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342402: a(n) is the n‑th perfect square k=n² such that
   among its proper divisors there is a subset of size τ(k)/2 whose sum
   equals k. This implementation tests successive squares and
   checks the property by a depth‑first search with pruning. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* maximum number of divisors that may occur for the considered range */
#define MAX_DIVS 20000

/* comparison function for qsort on seq_t values */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    return (*x > *y) ? 1 : 0;
}

/* generate all divisors of n² into buf[0..tau-1], tau = product(2*e+1) */
static void rec_divs(size_t idx, seq_t cur,
                     const uint32_t *primes, const uint8_t *exps, size_t pcnt,
                     seq_t *buf, size_t *pos)
{
    if (idx == pcnt) {
        buf[(*pos)++] = cur;
        return;
    }
    uint32_t p   = primes[idx];
    uint8_t exp  = exps[idx] * 2;      /* exponent in n² */
    seq_t prod   = 1;
    for (uint8_t i = 0; i <= exp; ++i) {
        rec_divs(idx + 1, cur * prod,
                 primes, exps, pcnt, buf, pos);
        prod *= p;
    }
}

/* DFS to decide whether a subset of size need from divs[idx..m-1]
   sums to k.  Prefix and maxs give lower/upper bounds. */
static bool dfs(const seq_t *divs, size_t m, size_t half,
                size_t idx, size_t need, seq_t sum, seq_t k,
                const seq_t *prefix, const seq_t *maxs)
{
    if (need == 0) return sum == k;
    if (idx == m) return false;
    if (need > m - idx) return false;

    /* minimal possible sum of the next 'need' smallest divisors */
    seq_t min_possible = prefix[idx + need] - prefix[idx];
    if (sum + min_possible > k) return false;

    /* maximal possible sum using the largest 'need' divisors */
    seq_t max_possible = maxs[need];
    if (sum + max_possible < k) return false;

    /* include divs[idx] */
    if (dfs(divs, m, half, idx + 1, need - 1,
            sum + divs[idx], k, prefix, maxs))
        return true;

    /* skip divs[idx] */
    if (dfs(divs, m, half, idx + 1, need,
            sum, k, prefix, maxs))
        return true;

    return false;
}

/* test whether k = n² satisfies the property */
static bool is_term(uint64_t n)
{
    /* factor n */
    uint32_t primes[32];
    uint8_t  exps[32];
    size_t   pcnt = 0;
    uint64_t x = n;

    while (x % 2 == 0) {
        if (pcnt == 0 || primes[pcnt - 1] != 2) {
            primes[pcnt] = 2;
            exps[pcnt]   = 0;
            pcnt++;
        }
        exps[pcnt - 1]++;
        x /= 2;
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        while (x % p == 0) {
            if (pcnt == 0 || primes[pcnt - 1] != p) {
                primes[pcnt] = p;
                exps[pcnt]   = 0;
                pcnt++;
            }
            exps[pcnt - 1]++;
            x /= p;
        }
    }
    if (x > 1) {
        primes[pcnt] = (uint32_t)x;
        exps[pcnt]   = 1;
        pcnt++;
    }

    /* total number of divisors of n² */
    size_t tau = 1;
    for (size_t i = 0; i < pcnt; ++i) tau *= (size_t)(2 * exps[i] + 1);

    if (tau > MAX_DIVS) return false;     /* safety */

    /* generate all divisors of n² */
    static seq_t all_divs[MAX_DIVS];
    size_t pos = 0;
    rec_divs(0, (seq_t)1, primes, exps, pcnt, all_divs, &pos);
    if (pos != tau) return false;         /* sanity check */

    /* sort divisors */
    qsort(all_divs, tau, sizeof(seq_t), cmp_seq_t);

    size_t m = tau - 1;          /* number of proper divisors */
    if (m == 0 || (m & 1)) return false;

    size_t half = m / 2;

    /* prefix sums of divisors (for lower bound) */
    static seq_t prefix[MAX_DIVS + 1];
    prefix[0] = 0;
    for (size_t i = 0; i < m; ++i) prefix[i + 1] = prefix[i] + all_divs[i];

    /* sums of the largest 'i' divisors (for upper bound) */
    static seq_t maxs[MAX_DIVS / 2 + 1];
    maxs[0] = 0;
    for (size_t i = 1; i <= half; ++i)
        maxs[i] = maxs[i - 1] + all_divs[m - i];

    seq_t k = (seq_t)n * (seq_t)n;
    return dfs(all_divs, m, half, 0, half, (seq_t)0, k, prefix, maxs);
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    static seq_t *seqtab = NULL;
    static size_t seqlen = 0, seqcap = 0;
    static uint64_t current_s = 1;   /* root of the current square being tested */

    if (n == 0) return 0;        /* safety */
    while (seqlen < (size_t)n) {
        ++current_s;
        if (is_term(current_s)) {
            seq_t val = (seq_t)current_s * (seq_t)current_s;
            if (seqlen == seqcap) {
                seqcap = seqcap ? seqcap * 2 : 16;
                seqtab = (seq_t *)realloc(seqtab, seqcap * sizeof(seq_t));
            }
            seqtab[seqlen++] = val;
        }
    }
    return seqtab[n - 1];
}
