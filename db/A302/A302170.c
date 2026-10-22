/*
 * A302170  Irregular triangle T(n,k) read by rows: first row is 1, n-th row
 *          (n > 1) lists distinct prime factors of n in decreasing order.
 * https://oeis.org/A302170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A302170: read the rows consisting of distinct
 * prime factors (in decreasing order) of each integer,
 * with the first row {1}.  The sequence is indexed from 1. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build.  seq_t is an
 * unsigned integer type of arbitrary width; idx_t is unsigned
 * 64‑bit. */
seq_t a(idx_t n)
{
    /* Static cache for cumulative counts of terms
     * up to each row.  prefix[0] = 0. */
    static idx_t *prefix = NULL;
    static size_t cap = 0;
    static size_t len = 0;      /* last filled index in prefix (row number) */

    /* Initialise the prefix array on first call */
    if (prefix == NULL) {
        cap = 16;
        prefix = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!prefix) return 0;            /* allocation failure → zero */
        prefix[0] = 0;
        len = 0;
    }

    /* Find the row r such that a(n) lies in that row.
     * Extend the prefix array as necessary. */
    idx_t cumulative = prefix[len];
    size_t r = len + 1;
    while (1) {
        /* Compute the length of row r (number of distinct prime factors,
         * or 1 for r == 1). */
        size_t len_r;
        if (r == 1) {
            len_r = 1;
        } else {
            uint64_t temp = (uint64_t)r;
            size_t cnt = 0;
            if ((temp & 1ULL) == 0) {
                /* include 2 once */
                ++cnt;
                while ((temp & 1ULL) == 0) temp >>= 1;
            }
            for (uint64_t p = 3; p <= temp / p; p += 2) {
                if (temp % p == 0) {
                    ++cnt;
                    while (temp % p == 0) temp /= p;
                }
            }
            if (temp > 1) ++cnt;
            len_r = cnt;
        }

        cumulative += (idx_t)len_r;

        /* Ensure enough capacity in the prefix array. */
        if (r >= cap) {
            size_t newcap = cap * 2;
            if (newcap < r + 1) newcap = r + 1;
            idx_t *tmp = (idx_t *)realloc(prefix, newcap * sizeof(idx_t));
            if (!tmp) return 0;        /* allocation failure */
            prefix = tmp;
            cap = newcap;
        }
        prefix[r] = cumulative;

        if (n <= cumulative) break;   /* found the row */
        ++r;
    }

    /* Compute position within row r (0‑based). */
    idx_t pos = n - prefix[r - 1] - 1;

    /* Special case: first row. */
    if (r == 1) return (seq_t)1;

    /* Determine the distinct prime factors of r in decreasing order. */
    uint64_t temp = (uint64_t)r;
    uint64_t primes[64];
    size_t plen = 0;

    if ((temp & 1ULL) == 0) {
        primes[plen++] = 2;
        while ((temp & 1ULL) == 0) temp >>= 1;
    }
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            primes[plen++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes[plen++] = temp;

    /* primes are stored in increasing order; return the
     * (plen-1-pos)-th element to get decreasing order. */
    return (seq_t)primes[plen - 1 - pos];
}
