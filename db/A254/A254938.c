/*
 * A254938  Fundamental positive solution x = x1(n) of the first class of the
 *          Pell equation x^2 - 2*y^2 = -A007522(n), n >= 1 (primes congruent
 *          to 7 mod 8).
 * https://oeis.org/A254938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 906, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A254938: a(n) = the smallest positive x for which
   x^2 - 2*y^2 = -p, where p is the n-th prime ≡ 7 (mod 8). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

/* Cache of primes congruent to 7 (mod 8).  The array holds seq_t values. */
static seq_t *p7_cache   = NULL;  /* array of primes */
static size_t p7_used   = 0;      /* how many primes are cached */
static size_t p7_cap    = 0;      /* current capacity of the array */
static uint64_t cand     = 7;     /* next candidate to test (≡ 7 mod 8) */

/* Ensure that p7_cache contains at least n primes congruent to 7 (mod 8). */
static void
ensure_primes(idx_t n)
{
    while (p7_used < n) {
        /* Grow the cache if needed. */
        if (p7_used == p7_cap) {
            size_t new_cap = (p7_cap == 0) ? 16 : p7_cap * 2;
            seq_t *tmp = (seq_t *)realloc(p7_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            p7_cache = tmp;
            p7_cap   = new_cap;
        }

        /* Test the current candidate. */
        if (is_prime((idx_t)cand))
            p7_cache[p7_used++] = (seq_t)cand;

        cand += 8;   /* next number ≡ 7 (mod 8) */
    }
}

/* Returns the n‑th term of A254938 for n ≥ 1. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Obtain the n‑th prime ≡ 7 (mod 8). */
    ensure_primes(n);
    seq_t p = p7_cache[n - 1];    /* p is the required prime */

    /* Search for the minimal solution (x, y) with x>0. */
    seq_t y = 1;
    for (;;) {
        seq_t y2 = y * y;            /* y²  */
        seq_t lhs = y2 * 2;           /* 2·y² */
        if (lhs <= p)                /* RHS would be negative */
            ++y;
        else {
            seq_t diff = lhs - p;     /* RHS = 2·y² - p = x² */
            seq_t x = floor_sqrt(diff);
            if (x * x == diff)       /* perfect square found */
                return x;
            ++y;
        }
    }
}
