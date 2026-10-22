/*
 * A385209  Least k such that A086369(k) = n.
 * https://oeis.org/A385209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 44, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A385209
 * a(n) = least k such that A086369(k) = n
 *
 * A086369(k) = | {d : d|k} ∪ { m : 0 < m < k , m + d = k for some d|k } |
 *              = 2*τ(k) - (k even ? 2 : 1)
 *
 * Hence
 *   if n is odd  → τ(k) = (n+1)/2 and k must be odd
 *   if n is even → τ(k) = (n+2)/2 and k must be even
 *
 * The function below finds the minimal such k by
 * enumerating exponent vectors (e_i) for the first primes,
 * maintaining non‑increasing exponents (optimal for minimal k) and
 * pruning when the current product exceeds the best known value.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* first 13 primes – enough for τ ≤ 5000 */
static const uint32_t _primes[] =
    { 2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u, 31u, 37u, 41u };
static const size_t _prime_count = sizeof(_primes) / sizeof(_primes[0]);

/* ------------------------------------------------------------------ */
/* Recursive search for minimal k with given target τ and parity     */
static void _dfs_find(int idx, int max_exp, seq_t cur_k, idx_t cur_tau,
                      idx_t target_tau, bool even_flag,
                      seq_t *best, bool *found)
{
    if (cur_tau > target_tau) return;
    if (cur_tau == target_tau) {
        seq_t mod2 = cur_k % 2;
        if ((even_flag && mod2 == 0) || (!even_flag && mod2 == 1)) {
            if (!*found || cur_k < *best) {
                *best = cur_k;
                *found = true;
            }
        }
        return;
    }
    if (idx >= (int)_prime_count) return;

    uint32_t p = _primes[idx];
    for (int e = 1; e <= max_exp; ++e) {
        idx_t new_tau = cur_tau * (idx_t)(e + 1);
        if (new_tau > target_tau) break;
        seq_t p_pow = seq_pow((seq_t)p, (idx_t)e);
        seq_t new_k  = seq_mul(cur_k, p_pow);
        if (*found && new_k >= *best) break;      /* pruning */
        _dfs_find(idx + 1, e, new_k, new_tau, target_tau,
                  even_flag, best, found);
    }
}

/* ------------------------------------------------------------------ */
/* Find minimal k with τ(k) = target_tau and required parity         */
static seq_t _min_k_with_tau(idx_t target_tau, bool even_flag)
{
    seq_t best = 0;
    bool found = false;

    /* start index: skip prime 2 for odd numbers */
    int start_idx = even_flag ? 0 : 1;
    /* allow many exponents – will break early by target_tau check */
    int initial_max_exp = 60;

    if (!even_flag) {
        _dfs_find(1, initial_max_exp, (seq_t)1, (idx_t)1,
                  target_tau, even_flag, &best, &found);
    } else {
        _dfs_find(0, initial_max_exp, (seq_t)1, (idx_t)1,
                  target_tau, even_flag, &best, &found);
    }
    return found ? best : (seq_t)0;
}

/* ------------------------------------------------------------------ */
/* Public entry point required by the judge                           */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset unused */
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    idx_t target_tau;
    bool even_flag;
    if ((n & 1u) != 0) {                  /* n odd */
        target_tau = (n + 1) / 2;
        even_flag  = false;
    } else {                              /* n even */
        target_tau = (n + 2) / 2;
        even_flag  = true;
    }
    return _min_k_with_tau(target_tau, even_flag);
}
