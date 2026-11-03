/*
 * A273163  P-defects p - N(p) of the congruence y^2 == x^3 + x^2 + 4*x + 4
 *          (mod p) for primes p, where N(p) is the number of solutions given
 *          for p = prime(n) by A272207(n).
 * https://oeis.org/A273163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 613, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9988 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 *  OEIS A273163 – P‑defects of the elliptic curve
 *
 *  a(n) = prime(n) – N(prime(n))
 *  where N(p) is the number of solutions (x,y)∈F_p² of
 *      y^2 ≡ x^3 + x^2 + 4x + 4 (mod p)
 *
 *  The implementation counts the solutions by brute force:
 *      for each x∈{0,…,p−1} compute RHS = (x^3+x^2+4x+4) mod p.
 *      If RHS=0  → one solution for y.
 *      If RHS≠0  → use the Legendre symbol (jacobi_symbol) to test
 *                 whether RHS is a quadratic residue; if so,
 *                 two solutions for y, otherwise none.
 *
 *  Complexity:  O(p) arithmetic operations per call.
 *
 *********************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

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

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Counts the number of solutions of y^2 = x^3 + x^2 + 4x + 4 (mod p) */
static seq_t
count_solutions(uint64_t p)
{
    seq_t count = 0;
    for (uint64_t x = 0; x < p; ++x) {
        /* Compute RHS modulo p */
        uint64_t xx = (x * x) % p;          /* x^2 */
        uint64_t x3 = (xx * x) % p;         /* x^3 */
        uint64_t rhs = (x3 + xx) % p;       /* x^3 + x^2 */
        rhs = (rhs + (4 * x) % p) % p;      /* + 4x */
        rhs = (rhs + 4) % p;                /* + 4 */

        if (rhs == 0) {
            count += 1;                    /* y ≡ 0 is the only solution */
        } else {
            int ls = jacobi_symbol(rhs, p); /* Legendre symbol */
            if (ls == 1) {
                count += 2;                /* two square roots exist */
            }
        }
    }
    return count;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* prime(n) – count_solutions(prime(n)) */
    uint64_t p = get_prime(n);
    seq_t Np = count_solutions(p);
    return (seq_t)p - Np;
}
