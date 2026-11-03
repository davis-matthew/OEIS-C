/*
 * A335435  Triangle read by rows: T(n,k) is the least degree of a polynomial
 *          interpolating (i, prime(i)) for i from k to n.
 * https://oeis.org/A335435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10011, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 112 terms through n = 8391 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A335435 – T(n,k): minimal degree of a polynomial interpolating
   (i,prime(i)) for i = k … n.  The sequence a(n) is the triangle read
   by rows: a(1)=0, a(2)=1, a(3)=0, a(4)=2, …  */

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

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n=0, return 0 as safe default */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n) r++;

    /* Column k within that row (1‑based) */
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t k = n - prev_tri;            /* k = column position */
    idx_t len = r - k + 1;             /* number of points to interpolate */

    /* Gather the prime values for indices k … r */
    uint64_t *pr = (uint64_t *)malloc(len * sizeof(uint64_t));
    if (!pr) return 0; /* memory failure */

    for (idx_t i = 0; i < len; ++i) {
        pr[i] = (uint64_t)get_prime(k + i);
    }

    /* Finite‑difference test for constant differences */
    uint64_t *cur = pr;
    idx_t sz = len;
    seq_t degree = 0;

    while (sz > 1) {
        idx_t next_len = sz - 1;
        uint64_t *next = (uint64_t *)malloc(next_len * sizeof(uint64_t));
        if (!next) { free(cur); return degree; }

        uint64_t first = cur[1] - cur[0];
        next[0] = first;
        bool const_flag = true;

        for (idx_t i = 1; i < next_len; ++i) {
            uint64_t diff = cur[i + 1] - cur[i];
            next[i] = diff;
            if (diff != first) const_flag = false;
        }

        free(cur);
        cur = next;
        sz = next_len;
        degree++;

        if (const_flag) break;
    }

    free(cur);
    return (seq_t)degree;
}
