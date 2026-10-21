/*
 * A269781  a(n) is the smallest k different from n such that (n, k) is an
 *          amicably refactorable pair (see the comments).
 * https://oeis.org/A269781
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k≠n such that numdiv(n) divides k and
 *            numdiv(k) divides n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 36, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   big_pow, divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

static const int PRIMES[15] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};

/* Recursive search for the minimal k with divisor count d_k
   and divisible by m.  req_exp[i] is the minimal exponent for PRIMES[i]. */
static void dfs_min_k(int idx, seq_t rem, seq_t val,
                      const int *req_exp, seq_t *best)
{
    if (rem == 1)
    {
        /* All remaining required exponents must already be satisfied. */
        for (int j = idx; j < 15; ++j)
            if (req_exp[j] > 0) return;
        if (*best == 0 || val < *best) *best = val;
        return;
    }
    if (idx >= 15) return;

    int req = req_exp[idx];
    /* e must be at least required exponent. */
    for (int e = req; e <= (int)rem - 1; ++e)   /* e+1 ≤ rem */
    {
        if (rem % (e + 1) != 0) continue;

        seq_t pow_val = (e > 0) ? big_pow((seq_t)PRIMES[idx], (seq_t)e) : 1;
        seq_t new_val = val * pow_val;

        if (*best != 0 && new_val >= *best) {
            if (e == 0) continue;          /* other e may give smaller val */
            else break;                     /* larger e will only increase val */
        }

        dfs_min_k(idx + 1, rem / (e + 1), new_val, req_exp, best);
    }
}

/* Minimal k with numdiv(k)=d_k and divisible by m.  Returns 0 if none. */
static seq_t minimal_k(seq_t m, seq_t d_k)
{
    if (d_k <= 1) return 0;   /* k would be 1 which cannot satisfy m|k for m≥2 */

    /* Factor m to obtain minimal required exponents. */
    uint64_t tmp_pr[15], tmp_ex[15];
    size_t cnt = 0;
    factor_u64((uint64_t)m, tmp_pr, tmp_ex, &cnt);

    int req[15] = {0};
    for (size_t i = 0; i < cnt; ++i) {
        int p = (int)tmp_pr[i];
        int e = (int)tmp_ex[i];
        for (int j = 0; j < 15; ++j)
            if (PRIMES[j] == p) { req[j] = e; break; }
    }

    seq_t best = 0;
    dfs_min_k(0, d_k, (seq_t)1, req, &best);
    return best;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* defined only for n≥3 */
    uint64_t nn = (uint64_t)n;
    seq_t best_k = 0;

    seq_t d_n = divisor_count_u64(nn);

    /* Iterate over all divisors of n as possible divisor counts of partner. */
    for (uint64_t dk = 1; dk <= nn; ++dk) {
        if (nn % dk != 0) continue;
        seq_t cand = minimal_k(d_n, (seq_t)dk);
        if (cand == 0) continue;
        if (cand == n) continue;
        if (best_k == 0 || cand < best_k) best_k = cand;
    }
    return best_k;
}
