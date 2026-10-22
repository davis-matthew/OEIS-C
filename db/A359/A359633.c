/*
 * A359633  a(n) is the least prime > a(n-1) such that a(n-1) and a(n) are
 *          quadratic residues mod each other.
 * https://oeis.org/A359633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2402, u32 n <= 623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 2;
   For n>1, a(n) is the smallest prime > a(n-1) such that a(n-1) and a(n)
   are quadratic residues modulo each other.  The algorithm builds a
   list of the sequence terms on demand, reusing all previously computed
   values (increasing or random access).  Prime tests use the supplied
   is_prime_big function and quadratic-residue checks exploit the
   Legendre symbol via modular exponentiation and special cases for 2. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

/* Return true if `prev` and `cand` are quadratic residues modulo one another. */
static inline bool residues_ok(seq_t prev, seq_t cand)
{
    /* Special case when one of the primes is 2.  2 is a quadratic residue
       mod an odd prime q iff q ≡ ±1 (mod 8). */
    if (prev == (seq_t)2) {
        seq_t r = cand % (seq_t)8;
        return (r == (seq_t)1) || (r == (seq_t)7);
    }
    if (cand == (seq_t)2) {
        seq_t r = prev % (seq_t)8;
        return (r == (seq_t)1) || (r == (seq_t)7);
    }

    /* If both primes are ≡3 (mod 4), reciprocity forbids them being mutual
       quadratic residues. */
    if ((prev % (seq_t)4) == (seq_t)3 && (cand % (seq_t)4) == (seq_t)3)
        return false;

    /* Euler's criterion: p^((q-1)/2) mod q == 1 ⇔ p is a quadratic residue mod q. */
    seq_t exp_seq = (cand - (seq_t)1) / (seq_t)2;
    uint64_t exp = (uint64_t)exp_seq;    /* safe for the tested range (≤ 2^64‑1) */
    seq_t r = mod_pow(prev, exp, cand);
    return r == (seq_t)1;
}

/* Global memoisation storage. */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t sz    = 0;

/* Ensure the cache can hold at least `needed` terms. */
static inline void ensure_cap(size_t needed)
{
    if (needed <= cap)
        return;
    size_t newcap = cap ? cap << 1 : 32;
    if (newcap < needed)
        newcap = needed;
    seq_t *newarr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = newarr;
    cap   = newcap;
}

/* Return the nth term of the sequence (1‑based).  Undefined for n<1
   – the caller must never request n==0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* safe placeholder */

    /* Extend cache up to n (inclusive). */
    if (n > sz) {
        ensure_cap((size_t)n);
        /* Initialise the very first term if this is the first call. */
        if (sz == 0) {
            cache[0] = (seq_t)2;
            sz = 1;
        }

        /* Compute missing terms. */
        for (size_t i = sz; i < (size_t)n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t cand = prev + (seq_t)1;

            /* Start at the next odd number if needed. */
            if (cand <= (seq_t)2)
                cand = (seq_t)3;
            else if ((cand & (seq_t)1) == (seq_t)0)
                ++cand;   /* make odd */

            /* Search for the next qualifying prime. */
            for (;;) {
                if (!is_prime_big(cand)) {
                    cand += (seq_t)2;   /* skip to next odd candidate */
                    continue;
                }
                if (residues_ok(prev, cand)) {
                    cache[i] = cand;
                    break;
                }
                cand += (seq_t)2;
            }
        }
        sz = n;
    }
    return cache[n - 1];
}
