/*
 * A339116  Triangle of all squarefree semiprimes grouped by greater prime
 *          factor, read by rows.
 * https://oeis.org/A339116
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1276, u32 n <= 1276, u64 n <= 1276,
 *            u128 n <= 1276, big any n
 * Validated: standard, pass, 71 terms through n = 1243 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: A339116 – Triangle of all squarefree semiprimes grouped
   by greater prime factor, read by rows.
   a(n) = prime(i) * prime(j)  where a(n) is the nth term (n ≥ 2),
   i > j, and the pair (i,j) is the position of the term in the
   lower‑triangular table read row by row (row 2 starts with i = 2). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* Return the nth term of A339116 (offset 2). */
seq_t a(idx_t n)
{
    if (n < 2)               /* undefined for indices below the offset */
        return 0;

    /* Zero–based offset inside the triangle. */
    idx_t m = n - 2;

    /* Find row index i such that (i-1)*i/2 > m  (minimal i).  */
    idx_t i = 1;
    while ((i - 1) * i / 2 <= m)
        i++;

    /* Number of elements preceding this row. */
    idx_t prev_total = (i - 2) * (i - 1) / 2;

    /* Column index j within the row (1‑based). */
    idx_t j = m - prev_total + 1;

    /* The value is the product of the j‑th and i‑th primes. */
    seq_t p_i = (seq_t)get_prime(i);
    seq_t p_j = (seq_t)get_prime(j);
    return p_i * p_j;
}
