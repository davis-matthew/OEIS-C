/*
 * A323377  Square array read by ascending antidiagonals: T(n,k) =
 *          Kronecker(prime(n)/prime(k)), n, k >= 1.
 * https://oeis.org/A323377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = Kronecker(p(n)/p(k)); a(n) is the value of T at the
 *            n‑th position when the infinite square array T is read by
 *            ascending antidiagonals with the entries on each antidiagonal
 *            ordered with i decreasing
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    idx_t cum = 1;                 /* cumulative elements up to current diagonal */
    while (cum < n) {
        ++d;
        cum += d;
    }
    idx_t prev   = cum - d;        /* cumulative elements up to previous diagonal */
    idx_t offset = n - prev;       /* 1‑based position within this diagonal */

    idx_t i = d - offset + 1;      /* row index (descending order on the diagonal) */
    idx_t j = offset;              /* column index */

    uint32_t pi = get_prime(i);    /* prime(i) */
    uint32_t pj = get_prime(j);    /* prime(j) */

    int symbol;
    if (pj == 2) {
        /* Kronecker symbol with denominator 2 */
        if ((pi & 1) == 0) {          /* pi even */
            symbol = 0;
        } else {
            uint32_t r = pi % 8;
            symbol = (r == 1 || r == 7) ? 1 : -1;
        }
    } else {
        /* pj is an odd prime; use Jacobi symbol */
        symbol = jacobi_symbol((uint64_t)pi, (uint64_t)pj);
    }
    return (seq_t)symbol;
}
