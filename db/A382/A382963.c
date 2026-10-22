/*
 * A382963  Prime index gaps between consecutive full reptend primes.
 * https://oeis.org/A382963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi(r(n+1)) - pi(r(n)) where r(k) is the k‑th full reptend
 *            prime (prime p with 10 as a primitive root mod p). The sequence
 *            starts at n = 1. The function memoises all primes and full
 *            reptend primes found so far, so any order of calls is supported
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- platform & util includes --------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#if defined(__cplusplus)
extern "C" {
#endif
#include "../utils/powmod64.h"  /* uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod); */
#if defined(__cplusplus)
}
#endif

/* ---------- static state ------------------------------------------------- */
static uint64_t *primes      = NULL;      /* all primes discovered so far   */
static size_t    primes_cap  = 0;
static size_t    primes_len  = 0;

static uint64_t *fr_idx      = NULL;      /* prime index (1‑based) of each full reptend prime */
static size_t    fr_cap      = 0;
static size_t    fr_len      = 0;

static uint64_t  next_candidate = 2;       /* next integer to test for primality */

/* ---------- helpers to grow dynamic arrays ----------------------------- */
static void ensure_primes_cap(size_t min)
{
    if (primes_cap >= min) return;
    size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
    while (new_cap < min) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes      = tmp;
    primes_cap  = new_cap;
}

static void ensure_fr_cap(size_t min)
{
    if (fr_cap >= min) return;
    size_t new_cap = (fr_cap == 0) ? 16 : fr_cap * 2;
    while (new_cap < min) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(fr_idx, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    fr_idx      = tmp;
    fr_cap      = new_cap;
}

/* ---------- check whether a prime p is a full reptend prime -------------- */
static bool is_full_reptend(uint64_t p)
{
    if (p <= 2) return false;   /* 2 is not a full reptend prime */
    if (p == 5) return false;   /* 5 is also not a full reptend prime */

    uint64_t n   = p - 1;                    /* order must divide p‑1 */
    uint64_t tmp = n;

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t q = primes[i];
        if (q * q > tmp) break;
        if (tmp % q == 0) {
            /* q is a distinct prime factor of n */
            uint64_t exp = n / q;
            if (powmod64(10, exp, p) == 1) return false;
            while (tmp % q == 0) tmp /= q;
        }
    }
    if (tmp > 1) {                            /* leftover prime factor */
        uint64_t exp = n / tmp;
        if (powmod64(10, exp, p) == 1) return false;
    }
    return true;
}

/* ---------- main sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* behaviour below offset is irrelevant */

    /* Ensure we have at least n+1 full reptend primes (indices 0..n) */
    while (fr_len <= (size_t)n) {
        uint64_t p = next_candidate;
        bool isPrime = true;

        /* Skip even composites >2 */
        if (p > 2 && (p & 1ULL) == 0) isPrime = false;

        if (isPrime) {
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t q = primes[i];
                if (q * q > p) break;
                if (p % q == 0) { isPrime = false; break; }
            }
        }

        if (isPrime) {
            /* store prime */
            ensure_primes_cap(primes_len + 1);
            primes[primes_len++] = p;

            /* test full‑reptend property */
            if (is_full_reptend(p)) {
                ensure_fr_cap(fr_len + 1);
                /* prime index is primes_len (1‑based) */
                fr_idx[fr_len++] = (uint64_t)primes_len;
            }
        }

        ++next_candidate;
    }

    /* a(n) = pi(r(n+1)) – pi(r(n))  = fr_idx[n] – fr_idx[n‑1]   */
    uint64_t diff = fr_idx[(size_t)n] - fr_idx[(size_t)(n - 1)];
    return (seq_t)diff;
}
