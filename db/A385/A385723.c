/*
 * A385723  Numbers k such that m^m == m (mod k) where m = ceiling(k/2).
 * https://oeis.org/A385723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A385723: numbers k such that (ceil(k/2))^(ceil(k/2)) ≡ ceil(k/2) mod k
 *
 *  This implementation generates the sequence by testing successive k.
 *  Even k with odd ceil(k/2) always satisfy the congruence; all other
 *  odd k are tested with a modular exponentiation.
 *
 *  @require  seq_t   – unsigned integer type (8‑32‑64‑128 bits or big int)
 *  @require  idx_t   – unsigned 64‑bit integer
 *  @require  seq_t operations support + - * / % and bitwise ops
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

/* --- modular exponentiation, implemented with seq_t arithmetic ------------------ */
static seq_t seq_mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t res = 1 % mod;          /* handle possible mod=1, but k>1 here           */
    seq_t b   = base % mod;       /* reduce base to avoid overflow                */
    while (exp) {
        if (exp & 1ULL)
            res = (res * b) % mod;
        exp >>= 1ULL;
        if (exp)
            b = (b * b) % mod;
    }
    return res;
}

/* --- cache for already found terms ------------------------------------------- */
static seq_t *cache      = NULL;    /* array of found k values                     */
static size_t cache_len  = 0;       /* number of terms stored in cache            */
static size_t cache_cap  = 0;       /* allocated capacity of cache                */
static idx_t  next_k     = 1;       /* next integer to test                       */

/* --- main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Grow cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k_val = (seq_t)next_k;
        bool ok;

        /* k = 1 is always a term */
        if (next_k == 1ULL) {
            ok = true;
        }
        /* even k with odd ceil(k/2) (i.e., k ≡ 2 (mod 4)) always satisfy */
        else if ((next_k & 1ULL) == 0ULL &&
                 ((next_k >> 1) & 1ULL) == 1ULL) {
            ok = true;
        }
        /* otherwise test the congruence (only odd k remain) */
        else {
            seq_t m = (k_val + 1) / 2;                 /* ceil(k/2) */
            seq_t pow_mod = seq_mod_pow(m, (idx_t)m, k_val);
            ok = (pow_mod == m);
        }

        if (ok) {
            /* ensure there is space in the cache */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 1 : cache_cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;                   /* out of memory – not expected */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k_val;
        }

        ++next_k;
    }

    return cache[(size_t)n - 1];
}
