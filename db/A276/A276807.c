/*
 * A276807  Number of solutions of the congruence y^2 == x^3 - x^2 - 4*x + 4
 *          (mod p) as p runs through the primes.
 * https://oeis.org/A276807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions of y² == x³ – x² – 4x + 4 (mod
 *            prime(n))
 * Range:     u8 n <= 55, u16 n <= 6510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9502 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by brute‑force enumeration.  For each prime p = prime(n)
   the set of squares mod p is pre‑computed once, then for every x the
   right‑hand side is evaluated and the number of corresponding y's is
   accumulated.  This is O(p²) in the sense of two nested loops, but
   p ≤ 104 729 for n ≤ 10 000, so the routine runs in a few milliseconds. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Count solutions modulo a (prime) p.  p fits into uint64_t. */
static seq_t count_solutions(uint64_t p)
{
    /* array of quadratic residues modulo p */
    bool *sq = (bool *)malloc(p * sizeof(bool));
    if (!sq) return (seq_t)0;   /* OOM unlikely in the test harness */

    memset(sq, 0, p * sizeof(bool));
    for (uint64_t y = 0; y < p; ++y) {
        uint64_t r = (y * y) % p;
        sq[r] = true;
    }

    seq_t solutions = 0;

    for (uint64_t x = 0; x < p; ++x) {
        uint64_t x2 = (x * x) % p;          /* x² mod p */
        uint64_t rhs = (x2 * x) % p;        /* x³ mod p */
        rhs = (rhs + p - x2) % p;           /* subtract x² */
        rhs = (rhs + p - (4ULL * x) % p) % p;   /* subtract 4x */
        rhs = (rhs + 4) % p;                /* add 4 */

        if (rhs == 0)                      /* y = 0 only solution */
            ++solutions;
        else if (sq[rhs])                  /* quadratic residue → ±y */
            solutions += 2;
        /* otherwise no solution for this x */
    }

    free(sq);
    return solutions;
}

/* External function required by the OEIS framework */
seq_t a(idx_t n)
{
    /* The nth prime (1‑based) */
    uint64_t p = get_prime(n);
    return count_solutions(p);
}
