/*
 * A359633  a(n) is the least prime > a(n-1) such that a(n-1) and a(n) are
 *          quadratic residues mod each other.
 * https://oeis.org/A359633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2403, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9801 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod
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

#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
    seq_t r = powmod(prev, exp, cand);
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
    seq_t *newarr = mem_resize(seq_t, cache, newcap);
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
