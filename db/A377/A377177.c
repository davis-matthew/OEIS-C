/*
 * A377177  Primes p such that -7/2 is a primitive root modulo p.
 * https://oeis.org/A377177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 2506, u32 n <= 3713, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, factor_u64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches for primes p such that -7/2 is a primitive
 * root modulo p.  For each prime candidate p>2 it computes
 * g = (-7/2) mod p, skips the case g == 0 (which occurs only for
 * p = 7), factors p-1, and validates that g is a primitive root.
 * The n-th such prime (n ≥ 1) is returned. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/append_candidate.h"

/* Cache of primes already found. */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static uint64_t next_candidate = 3;   /* start after 2 with odd numbers */

/* Return the n-th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) { return 0; }          /* undefined for n = 0, but harmless */

    /* Keep searching until we have found at least n terms. */
    while (seq_len < (size_t)n) {
        uint64_t p = next_candidate;
        next_candidate += 2;           /* only odd candidates */

        /* skip non‑prime candidates */
        if (!is_prime((idx_t)p)) continue;

        /* Compute g = (-7/2) mod p.  For odd p, inv2 = (p+1)/2 is the
         * modular inverse of 2.  We perform the calculation using seq_t
         * to avoid overflow for larger builds. */
        seq_t inv2 = ((seq_t)p + 1) / 2;
        seq_t g    = (((seq_t)p - (seq_t)(7 % p)) * inv2) % (seq_t)p;

        /* Skip the trivial case g == 0 (only occurs for p = 7). */
        if (g == 0) continue;

        /* factor p-1 */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt;
        factor_u64(p - 1, primes, exps, &cnt);

        /* Check that g is a primitive root: for every prime factor q
         * of p-1, g^((p-1)/q) mod p must not be 1. */
        bool ok = true;
        for (size_t i = 0; i < cnt && ok; ++i) {
            uint64_t q = primes[i];
            if (mod_pow(g, (p - 1) / q, (seq_t)p) == 1)
                ok = false;
        }

        if (ok) {
            append_candidate(&seq, &seq_len, &seq_cap, (seq_t)p);
        }
    }

    return seq[n - 1];
}
