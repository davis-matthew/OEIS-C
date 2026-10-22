/*
 * A376422  Numbers m with largest nondivisor k <= m such that rad(k) | m is
 *          not powerful, where rad = A007497.
 * https://oeis.org/A376422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 8617, u32 n <= 8617, u64 n <= 8617,
 *            u128 n <= 8617, big any n
 * Helpers:   append_candidate, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A376422: sequence of m such that the largest nondivisor
   k ≤ m with rad(k) | m is NOT powerful. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"      /* factor_u64 */
#include "../utils/append_candidate.h" /* append_candidate */

/*------------- helper: depth‑first search for all numbers built
   from the prime factors of m -------------------------------------*/
static void dfs(const uint64_t *primes, const uint64_t *maxE,
                size_t pcnt, size_t pos, uint64_t cur, uint64_t m,
                uint64_t *best)
{
    if (pos == pcnt) {
        if (cur <= 1 || cur == m) return;          /* skip trivial or divisor */
        if (m % cur != 0 && cur > *best) *best = cur;
        return;
    }
    uint64_t p = primes[pos];
    uint64_t val = 1;                             /* p^e , e = 0 … */
    for (uint64_t e = 0; e <= maxE[pos]; ++e) {
        if (cur > m / val) break;                 /* cur*val would overflow m */
        dfs(primes, maxE, pcnt, pos + 1, cur * val, m, best);
        if (e == maxE[pos]) break;
        if (val > m / p) break;                   /* next multiplication would overflow */
        val *= p;
    }
}

/* find largest k ≤ m such that
     - k is not a divisor of m
     - rad(k) | m
     Return true and set *best if such k exists, otherwise false.  */
static bool find_largest_nondivisor(uint64_t m, uint64_t *best)
{
    if (m < 2) { *best = 0; return false; }

    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64(m, primes, exps, &pcnt);          /* factor m */

    if (pcnt < 2) { *best = 0; return false; }  /* single prime power → no eligible k */

    uint64_t maxExp[64];
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t val = p;
        uint64_t e = 1;
        while (val <= m / p) {                     /* largest e with p^e ≤ m */
            val *= p;
            ++e;
        }
        maxExp[i] = e;
    }

    uint64_t best_k = 0;
    dfs(primes, maxExp, pcnt, 0, 1, m, &best_k);

    if (best_k == 0) return false;
    *best = best_k;
    return true;
}

/* check if v is a powerful number (all prime exponents ≥ 2) */
static bool is_powerful(uint64_t v)
{
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64(v, primes, exps, &pcnt);
    for (size_t i = 0; i < pcnt; ++i)
        if (exps[i] == 1) return false;
    return true;
}

/*----- public implementation -----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for offset */

    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t next_m = 1;               /* current m being tested */

    while (seq_len < (size_t)n) {
        uint64_t best_k;
        if (find_largest_nondivisor(next_m, &best_k)) {
            if (!is_powerful(best_k))
                append_candidate(&seq, &seq_len, &seq_cap, (seq_t)next_m);
        }
        ++next_m;
    }

    return seq[n - 1];
}
