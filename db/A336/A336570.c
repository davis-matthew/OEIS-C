/*
 * A336570  Number of maximal sets of proper divisors d|n, d < n, all
 *          belonging to A130091 (numbers with distinct prime multiplicities)
 *          and forming a divisibility chain.
 * https://oeis.org/A336570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of maximal chains of proper divisors d|n (d<n) *
 *            each belonging to A130091 (distinct prime exponents), ordered by
 *            * divisibility. The chain is maximal iff it cannot be extended
 *            by * adding another eligible divisor at either end or in between
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Basic utilities                                                      */
/* --------------------------------------------------------------------- */

/* Factor a 64‑bit integer.  Returns the number of distinct prime factors.
 *  `pr` receives the prime bases and `ex` the corresponding exponents. */
static size_t factor_uint64(uint64_t n, uint64_t *pr, int *ex)
{
    size_t cnt = 0;
    uint64_t rem = n;
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            pr[cnt] = p;
            ex[cnt] = e;
            ++cnt;
        }
    }
    if (rem > 1) {
        pr[cnt] = rem;
        ex[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* Check whether a number has distinct prime exponents.
 *  1 is considered to satisfy the condition, but we exclude it
 *  when selecting eligible divisors. */
static bool has_distinct_exponents(uint64_t d)
{
    if (d <= 1) return true;   /* 1 has no exponents */
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factor_uint64(d, primes, exps);
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (exps[i] == exps[j]) return false;
    return true;
}

/* --------------------------------------------------------------------- */
/*  Main function                                                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) is defined as 1 */
    if (n == 1) return (seq_t)1;
    uint64_t N = (uint64_t)n;

    /* --------------------------------------------------------------- */
    /* 1. Generate all proper divisors of N                           */
    /* --------------------------------------------------------------- */
    uint64_t primes[64];
    int exps[64];
    size_t pf_cnt = factor_uint64(N, primes, exps);

    size_t cap = 16;
    size_t sz = 0;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!divs) return (seq_t)0;         /* allocation failed */
    divs[sz++] = 1;                     /* start with divisor 1 */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p   = primes[i];
        int     exp = exps[i];
        size_t  old_sz = sz;
        uint64_t pk = 1;
        for (int k = 1; k <= exp; ++k) {
            pk *= p;
            for (size_t j = 0; j < old_sz; ++j) {
                uint64_t v = divs[j] * pk;
                if (sz == cap) {
                    cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(divs, cap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return (seq_t)0; }
                    divs = tmp;
                }
                divs[sz++] = v;
            }
        }
    }

    /* --------------------------------------------------------------- */
    /* 2. Keep proper divisors belonging to A130091 and < N            */
    /* --------------------------------------------------------------- */
    uint64_t *eligible = (uint64_t *)malloc(sz * sizeof(uint64_t));
    if (!eligible) { free(divs); return (seq_t)0; }
    size_t m = 0;
    for (size_t i = 0; i < sz; ++i) {
        uint64_t d = divs[i];
        if (d == N) continue;             /* proper divisor only */
        if (d == 1) continue;             /* skip 1 to match OEIS data */
        if (has_distinct_exponents(d)) {
            eligible[m++] = d;
        }
    }
    free(divs);

    /* No eligible divisors → only the empty chain is possible. */
    if (m == 0) { free(eligible); return (seq_t)1; }

    /* --------------------------------------------------------------- */
    /* 3. Sort eligible divisors (ascending)                           */
    /* --------------------------------------------------------------- */
    for (size_t i = 1; i < m; ++i) {
        uint64_t key = eligible[i];
        size_t j = i;
        while (j > 0 && eligible[j-1] > key) {
            eligible[j] = eligible[j-1];
            --j;
        }
        eligible[j] = key;
    }

    /* --------------------------------------------------------------- */
    /* 4. Dynamic programming on the DAG of covering relations        */
    /*    (Only immediate divisibility edges are considered).         */
    /* --------------------------------------------------------------- */
    seq_t *dp = (seq_t *)calloc(m, sizeof(seq_t));
    if (!dp) { free(eligible); return (seq_t)0; }

    /* For each node compute dp[i] and whether it has an immediate successor. */
    bool *hasSucc = (bool *)calloc(m, sizeof(bool));
    if (!hasSucc) { free(eligible); free(dp); return (seq_t)0; }

    for (size_t i = 0; i < m; ++i) {
        bool hasPred = false;

        /* Scan all earlier nodes for immediate predecessors. */
        for (size_t j = 0; j < i; ++j) {
            if (eligible[i] % eligible[j] != 0) continue;

            bool immediate = true;
            for (size_t k = j + 1; k < i; ++k) {
                if (eligible[k] % eligible[j] == 0 &&
                    eligible[i] % eligible[k] == 0) {
                    immediate = false;
                    break;
                }
            }
            if (immediate) {
                dp[i] += dp[j];
                hasPred = true;
            }
        }
        if (!hasPred) dp[i] = (seq_t)1;   /* minimal element */

        /* Scan later nodes for immediate successors to record maximality. */
        for (size_t j = i + 1; j < m; ++j) {
            if (eligible[j] % eligible[i] != 0) continue;

            bool immediate = true;
            for (size_t k = i + 1; k < j; ++k) {
                if (eligible[k] % eligible[i] == 0 &&
                    eligible[j] % eligible[k] == 0) {
                    immediate = false;
                    break;
                }
            }
            if (immediate) {
                hasSucc[i] = true;
                break;                   /* no need to search further */
            }
        }
    }

    /* --------------------------------------------------------------- */
    /* 5. Sum dp[ i ] for all maximal nodes (no immediate successor). */
    /* --------------------------------------------------------------- */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < m; ++i) {
        if (!hasSucc[i]) result += dp[i];
    }

    free(eligible);
    free(dp);
    free(hasSucc);

    return result;
}
