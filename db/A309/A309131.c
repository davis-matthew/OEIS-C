/*
 * A309131  Triangle read by rows: T(n, k) = (n - k)*prime(1 + k), with 0 <= k
 *          < n.
 * https://oeis.org/A309131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r, k) where r = smallest integer such that r(r+1)/2 ≥
 *            n, k = n - r(r-1)/2 - 1; T(r, k) = (r - k) * prime(1 + k)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset starts at 1 */

    /* Find the row r containing the n-th term. */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++r;
        cumulative += r;                    /* cumulative = r(r+1)/2 */
    }

    /* Position within the row. */
    idx_t prefix_prev = cumulative - r;     /* = r(r-1)/2 */
    idx_t k = n - prefix_prev - 1;          /* 0‑based index in the row */

    /* Compute the term: (r - k) * prime(1 + k). */
    seq_t prime_val = (seq_t)get_prime(k + 1);
    return (seq_t)(r - k) * prime_val;
}
