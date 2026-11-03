/*
 * A328739  Table of A(n,k) read by antidiagonals, where A(n,1)=2, and every
 *          n+1 consecutive terms in row n are pairwise coprime. Terms are
 *          chosen to be the least increasing value compatible with these
 *          constraints.
 * https://oeis.org/A328739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   this function computes the antidiagonal reading of table A(n,k).
 *            The value equals A(i,j) where (i,j) is the position in the
 *            triangular array corresponding to the sequence index n. The
 *            table is defined by: • A(n,1)=2; • for 1 ≤ k ≤ n+1,
 *            A(n,k)=prime(k); • for k>n+1, A(n,k) is the smallest integer...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

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

seq_t a(idx_t n) {
    /* Determine row i and column j from the antidiagonal index n. */
    idx_t s = 2;                         /* minimal sum i+j = 2 (1,1) */
    while ((s - 1) * s / 2 < n)          /* find minimal s with T(s-1) >= n */
        s++;
    idx_t tprev = (s - 2) * (s - 1) / 2; /* T(s-2) */
    idx_t i = n - tprev;                 /* row index */
    idx_t j = s - i;                     /* column index */

    /* Generate the first j elements of row i. */
    seq_t *row = mem_new(seq_t, (j + 1));
    if (!row) return 0; /* allocation failure – unlikely in contest */

    for (idx_t k = 1; k <= j; ++k) {
        if (k <= i + 1) {
            /* k <= i+1 so the value is the k-th prime. */
            row[k] = (seq_t)get_prime(k);
        } else {
            /* Find the smallest integer larger than the previous element
               that is coprime with the previous i elements. */
            seq_t cand = row[k - 1] + 1;
            for (;;) {
                bool ok = true;
                for (idx_t t = k - 1; t >= k - i; --t) {
                    if (gcd(cand, row[t]) != 1) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    row[k] = cand;
                    break;
                }
                cand++;
            }
        }
    }

    seq_t result = row[j];
    mem_free(row);
    return result;
}
