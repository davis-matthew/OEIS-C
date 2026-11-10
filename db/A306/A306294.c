/*
 * A306294  Least inverse of A162961.
 * https://oeis.org/A306294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k : A162961(k) = n}; found by a linear search over k,
 *            with A162961 computed by CRT over permutations of the maximal
 *            prime powers
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 829 (2026-11-08)
 * Helpers:   factor_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/powmod.h"

static seq_t A162961_modinv_pp(seq_t m, seq_t mod_i, uint64_t p)
{
    seq_t phi = mod_i - (mod_i / p);
    uint64_t ex = (uint64_t)(phi - (seq_t)1);
    return powmod(m, ex, mod_i);
}

static void A162961_dfs(const seq_t *mods, const uint64_t *primes, int P, int depth,
                        int *used, seq_t a_res, seq_t m_cur, seq_t *best)
{
    if (depth == P) {
        seq_t cand = a_res % m_cur;
        if (cand == 0) cand = m_cur;
        if (*best == 0 || cand < *best) *best = cand;
        return;
    }
    for (int i = 0; i < P; ++i) {
        if (!used[i]) {
            used[i] = 1;
            const seq_t mod_i = mods[i];
            const uint64_t prime_i = primes[i];
            seq_t offset = (seq_t)depth;
            seq_t rem = offset % mod_i;
            seq_t r = (rem == 0) ? (seq_t)0 : mod_i - rem;
            seq_t a_mod = a_res % mod_i;
            seq_t delta = (r + mod_i - a_mod) % mod_i;
            seq_t inv_m = A162961_modinv_pp(m_cur, mod_i, prime_i);
            seq_t t = (delta * inv_m) % mod_i;
            seq_t new_a = a_res + m_cur * t;
            seq_t new_m = m_cur * mod_i;
            A162961_dfs(mods, primes, P, depth + 1, used, new_a, new_m, best);
            used[i] = 0;
        }
    }
}

static seq_t A162961_a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    if (cnt == 1) return (seq_t)n;
    seq_t mods[64];
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t power = 1;
        for (uint64_t e = 0; e < exps[i]; ++e) power *= p;
        mods[i] = power;
    }
    int used[64] = {0};
    seq_t best = 0;
    A162961_dfs(mods, primes, (int)cnt, 0, used, (seq_t)0, (seq_t)1, &best);
    return best;
}

#define A306294_CACHE 65536
static seq_t A306294_cache[A306294_CACHE];
static idx_t A306294_ncache = 0;

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    for (idx_t k = 1;; ++k) {
        seq_t v;
        if (k <= A306294_ncache) {
            v = A306294_cache[k - 1];
        } else {
            v = A162961_a(k);
            if (k == A306294_ncache + 1 && k <= A306294_CACHE) {
                A306294_cache[k - 1] = v;
                A306294_ncache = k;
            }
        }
        if (v == target) return (seq_t)k;
    }
}
