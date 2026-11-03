/*
 * A375488  Numbers k such that k^(sigma(k) - k) == k (mod sigma(k)), where
 *          sigma = A000203.
 * https://oeis.org/A375488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375488: Numbers k such that k^(σ(k)-k) ≡ k (mod σ(k))
   Computed by testing successive integers and caching the results.
*/

#include <stdlib.h> /* malloc, realloc, free */
#include <stdint.h> /* uint64_t */
#include <stdbool.h>

/* Utility functions from the external library */
#include <string.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* modular exponentiation using 128‑bit intermediate.
   Works for any seq_t. */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t res = 1 % mod;
    base   %= mod;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * base % mod;
        exp >>= 1;
        if (exp)
            base = (unsigned __int128)base * base % mod;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  chk   = 1;                 /* next integer to test */

    /* Ensure at least n terms are computed */
    while (len < n) {
        /* Grow dynamically if needed */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            /* realloc returns void*, cast to seq_t* for C and C++ */
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) {
                /* Out‑of‑memory – terminate the program (undefined in this template) */
                abort();
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* Test the current integer */
        idx_t k = chk;
        seq_t sigma = sigma_of(k);
        seq_t k_seq = (seq_t)k;
        seq_t diff   = sigma - k_seq;            /* σ(k) - k, always ≥ 0 */
        /* Assume diff fits into uint64_t for the ranges we care about. */
        uint64_t exp = (uint64_t)diff;
        seq_t powmod = mod_pow(k_seq, exp, sigma);
        seq_t modk   = k_seq % sigma;

        if (powmod == modk) {
            cache[len++] = k_seq;
        }
        ++chk;
    }

    return cache[n - 1];   /* no bounds check – callers assume n ≥ 1 */
}
