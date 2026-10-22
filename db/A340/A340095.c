/*
 * A340095  Odd composite integers m such that A052918(m-J(m,29)) == 0 (mod m)
 *          and gcd(m,29)=1, where J(m,29) is the Jacobi symbol.
 * https://oeis.org/A340095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   U_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A340095:
   odd composite integers m with gcd(m,29)=1,
   and U_mod(m - Jacobi(m,29), m) == 0,
   where U_mod is the Lucas sequence U_n for parameters a=5, b=-1,
   and Jacobi(m,29) is the Jacobi symbol (m|29). */

/* The algorithm performs a lazy sieve: every call to a(n) expands the
   internal cache until it contains at least n terms.  The sequence can
   be generated in O(n log n) time, well below the required limit for
   n up to 10 000. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/U_mod.h"

/* ------------------------------------------------------------------ */
/*  Helper: deterministic primality test for uint64_t                */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Witnesses for 64‑bit integers */
    const uint64_t witnesses[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
         9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = 1, power = d, base = a;
        while (power) {
            if (power & 1) x = (uint64_t)((unsigned __int128)x * base % n);
            base = (uint64_t)((unsigned __int128)base * base % n);
            power >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (uint64_t)((unsigned __int128)x * x % n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Helper: Jacobi symbol (a|n) for odd n                              */
/* ------------------------------------------------------------------ */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0 || (n & 1) == 0) return 0;
    a %= n;
    int result = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n % 8;
            if (n_mod8 == 3 || n_mod8 == 5) result = -result;
        }
        uint64_t tmp = a; a = n; n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3) result = -result;
        a %= n;
    }
    return (n == 1) ? result : 0;
}

/* ------------------------------------------------------------------ */
/*  Helper: Lucas U_n modulo m for a=5, b=-1                        */

/* ------------------------------------------------------------------ */
/*  Cache of sequence terms                                            */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_terms = 0;
static uint64_t last_checked = 8;   /* next candidate will be 9 */

/* ------------------------------------------------------------------ */
/*  Ensure cache has space, expanding if necessary                   */
/* ------------------------------------------------------------------ */
static void cache_append(seq_t val) {
    if (cached_terms >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        /* cast suited for both C and C++ */
        cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cached_terms++] = val;
}

/* ------------------------------------------------------------------ */
/*  Generate terms until we have at least n terms in the cache       */
/* ------------------------------------------------------------------ */
static void generate_terms(idx_t n) {
    while (cached_terms < n) {
        uint64_t cand = last_checked + 1;
        if ((cand & 1) == 0) cand++;                  /* only odd numbers */
        last_checked = cand;

        if (cand % 29 == 0) continue;                 /* gcd(m,29)=1 */
        if (!is_prime_uint64(cand)) {                 /* must be composite */
            int jac = jacobi_symbol(cand, 29);         /* ±1 for coprime n */
            seq_t k = (seq_t)((jac == 1) ? (cand - 1) : (cand + 1));
            seq_t u = U_mod(k, (seq_t)cand);
            if (u == 0) cache_append((seq_t)cand);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public entry point                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;             /* undefined, but harmless             */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cached_terms = 0;
        last_checked = 8;
    }
    generate_terms(n);
    return cache[n - 1];
}
