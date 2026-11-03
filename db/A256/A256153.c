/*
 * A256153  Primitive prime factors of the cyclotomic polynomial sequence
 *          Phi(5,k) in the order in which they occur.
 * https://oeis.org/A256153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 296, u64 n <= 1191,
 *            u128 n <= 1191, big any n
 * Validated: standard, pass, 58 terms through n = 1150 (2026-11-03)
 * Helpers:   gcd, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primitive prime factors of Φ(5,k) = k⁴ + k³ + k² + k + 1 in the order
   in which they first appear as k increases.  For each k = 1, 2, … the
   value Φ(5,k) is factored (using Pollard‑Rho and trial division) and
   its distinct prime factors are examined in ascending order.  Any
   factor that has not appeared before is appended to a static array;
   the function then returns the n‑th element of that array.  The
   array is kept in static storage so that repeated calls are
   essentially O(1) after the first scan. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Pollard‑Rho: returns a non‑trivial factor of a composite  n (n > 1). */
/* It uses modular multiplication with 128‑bit intermediates to avoid
   overflow for φ(5,k) up to ~10¹⁶ (k ≤ 10⁴). */
static seq_t pollard_rho(seq_t n)
{
    if ((n & 1) == 0) return 2;          /* even */

    seq_t x = 2, y = 2, c = 1, d = 1;
    while (d == 1) {
        /* x = f(x), y = f(f(y)) with f(t)=t²+c mod n */
        x = (seq_t)(((__uint128_t)x * x + c) % n);
        y = (seq_t)(((__uint128_t)y * y + c) % n);
        y = (seq_t)(((__uint128_t)y * y + c) % n);

        seq_t diff = (x > y) ? (x - y) : (y - x);
        d = gcd(diff, n);

        if (d == n) {               /* failure – try a new c */
            ++c;
            x = y = 2;
            d = 1;
        }
    }
    return d;
}

/* Recursive factorisation of n.  The prime factors (with multiplicity)
   are appended to fac[0..cnt‑1].  The array size (here 64) is ample
   for the φ(5,k) values that arise for k ≤ 10⁴. */
static void factor_rec(seq_t n, seq_t *fac, size_t *cnt) {
    if (n == 1) return;
    if (isPrimeGeneric(n)) {
        fac[(*cnt)++] = n;
        return;
    }
    seq_t d = pollard_rho(n);
    factor_rec(d, fac, cnt);
    factor_rec(n / d, fac, cnt);
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    /* Static state kept across calls */
    static idx_t next_k = 1;            /* next k to process (inclusive) */
    static size_t found   = 0;          /* number of distinct primes found */
    static seq_t *seen   = NULL;        /* array of primes in order */
    static size_t seen_cap = 0;         /* capacity of 'seen' */

    /* Initialise storage on the first call */
    if (!seen) {
        seen_cap = 128;
        seen = mem_new(seq_t, seen_cap);
    }

    /* Compute new k's until we have at least n primes recorded */
    while (found < (size_t)n) {
        /* Compute Φ(5,k) = k⁴ + k³ + k² + k + 1 */
        seq_t k = (seq_t)next_k;
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        seq_t k4 = k3 * k;
        seq_t phi = k4 + k3 + k2 + k + 1;

        /* Factor φ(k) into primes (with multiplicity) */
        seq_t fac[64];
        size_t fac_cnt = 0;
        factor_rec(phi, fac, &fac_cnt);

        /* Sort the factor list so duplicate removal is easy */
        if (fac_cnt > 1) {
            for (size_t i = 1; i < fac_cnt; ++i)
                for (size_t j = i; j > 0 && fac[j-1] > fac[j]; --j) {
                    seq_t t = fac[j];
                    fac[j] = fac[j-1];
                    fac[j-1] = t;
                }
            /* Collapse equal neighbours to a unique list */
            size_t uniq = 1;
            for (size_t i = 1; i < fac_cnt; ++i)
                if (fac[i] != fac[uniq-1])
                    fac[uniq++] = fac[i];
            fac_cnt = uniq;
        }

        /* Record each new prime factor that has not appeared before */
        for (size_t i = 0; i < fac_cnt; ++i) {
            seq_t p = fac[i];
            bool already = false;
            for (size_t j = 0; j < found; ++j)
                if (seen[j] == p) { already = true; break; }
            if (!already) {
                if (found >= seen_cap) {
                    seen_cap *= 2;
                    seen = mem_resize(seq_t, seen, seen_cap);
                }
                seen[found++] = p;
            }
        }

        ++next_k;   /* next k to examine */
    }

    return seen[n-1];
}
