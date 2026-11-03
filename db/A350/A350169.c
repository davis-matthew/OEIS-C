/*
 * A350169  Write 1st prime and decrement 0 times, then write 2nd prime and
 *          decrement once, write 3rd prime and decrement twice, write 4th
 *          prime and decrement 3 times, etc ...
 * https://oeis.org/A350169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A350169
 *  a(n) is the element obtained by writing the r‑th prime p_r
 *  and then decreasing it by 1, r times.  The row containing
 *  a(n) is the smallest r such that r*(r+1)/2 ≥ n.
 *  a(n) = p_r - (n - T(r-1) - 1), where T(k)=k*(k+1)/2.  */

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

/* Computes a(n),  n ≥ 1  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n<1 */

    /* Find r with T(r-1) < n ≤ T(r)   (binary search) */
    idx_t lo = 1, hi = 2;
    /* grow hi until enough */
    while (((unsigned __int128)hi * (hi + 1) / 2) < n)
        hi <<= 1U;

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                    /* row index (1‑based) */

    /* start index of row r (0‑based) = T(r-1) */
    unsigned __int128 start128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t start = (idx_t)start128;

    /* column offset inside the row (0‑based) */
    uint64_t i = n - start - 1;

    /* r‑th prime */
    uint32_t p = get_prime(r);

    /* result */
    return (seq_t)p - (seq_t)i;
}
