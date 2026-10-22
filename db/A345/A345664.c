/*
 * A345664  Primes whose digit reversal is the product of two (not necessarily
 *          distinct) emirps.
 * https://oeis.org/A345664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   factor_u64, is_prime, is_prime_big, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes p such that the decimal digit
   reversal rev(p) is a product of two emirps (primes whose
   reversal is a different prime).  The term a(n) is the n‑th such
   prime in increasing order.  The function generates successive
   primes and tests each one by factorising rev(p).  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* utility functions from the provided library */
#include "../utils/rev.h"             /* idx_t rev(idx_t x); */
#include "../utils/factor_u64.h"       /* factor_u64(uint64_t n,
                                       uint64_t *pr, uint64_t *ex,
                                       size_t *cnt); */
#include "../utils/is_prime.h"        /* bool is_prime(idx_t x); */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n); */

/* ---------- internal cache for the sequence ---------- */
static seq_t *seq_cache   = NULL;
static size_t  seq_cache_len = 0;
static size_t  seq_cache_cap = 0;

/* append an element to the cache, resizing on demand */
static void seq_cache_append(seq_t v)
{
    if (seq_cache_len == seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
    seq_cache[seq_cache_len++] = v;
}

/* ---------- helper to test whether a prime is an emirp ---------- */
static inline bool is_emirp(uint64_t x)
{
    if (x <= 1) return false;
    idx_t revx = rev(x);          /* rev operates on idx_t = uint64_t */
    if (revx == x) return false;  /* palindromic primes are not emirps */
    /* x is prime by construction (from factorisation), only need to test rev(x) */
    return is_prime_big((seq_t)x) && is_prime_big((seq_t)revx);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* undefined for n==0; return 0 for safety */
    if (n == 0) return 0;

    /* generate terms until we have at least n of them */
    static uint64_t next_candidate = 2;   /* next number to test for primality */
    while (seq_cache_len < (size_t)n) {
        uint64_t p;

        /* find the next prime p */
        if (next_candidate == 2) {
            p = 2;
            next_candidate = 3;
        } else {
            /* skip even numbers */
            while (!is_prime(next_candidate))
                next_candidate += 2;
            p = next_candidate;
            next_candidate += 2;
        }

        /* candidate prime p: check its reversal */
        uint64_t revp = rev(p);

        /* factorise rev(p) */
        uint64_t pr[32];
        uint64_t ex[32];
        size_t cnt = 0;
        factor_u64(revp, pr, ex, &cnt);

        bool ok = false;
        if (cnt == 1 && ex[0] == 2) {
            /* rev(p) is a square of a prime e*e */
            uint64_t e = pr[0];
            if (is_emirp(e)) {
                ok = true;
            }
        } else if (cnt == 2 && ex[0] == 1 && ex[1] == 1) {
            /* rev(p) is the product of two distinct primes e1*e2 */
            uint64_t e1 = pr[0];
            uint64_t e2 = pr[1];
            if (is_emirp(e1) && is_emirp(e2)) {
                ok = true;
            }
        }

        if (ok) {
            seq_cache_append((seq_t)p);
        }
    }

    return seq_cache[n - 1];
}
