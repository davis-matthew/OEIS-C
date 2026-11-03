/*
 * A362017  a(n) is the leading prime in the n-th prime sublist defined in
 *          A348168.
 * https://oeis.org/A362017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = leading prime of the n-th prime sublist defined in
 *            A348168. A sublist starts at prime P[s]. Let Gprev = P[s] -
 *            P[s-1] (gap preceding the sublist; for s=1 use ∞) Gfirst =
 *            P[s+1] - P[s] (first gap inside the sublist). The sublist
 *            consists of consecutive primes beginning with P[s] while
 *            every...
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)             /* offset is 1; return 0 for invalid 0 */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)get_prime(1);   /* leading prime of first sublist */

    idx_t start  = 1;      /* current sublist start index (1‑based) */
    idx_t count  = 1;      /* number of sublists found so far */

    while (count < n) {
        /* Handle the first sublist [2] specially */
        if (start == 1) {
            start = 2;    /* next sublist begins at index 2 (prime 3) */
            count++;
            continue;
        }

        /* Current prime and previous prime of the sublist */
        uint64_t cur = (uint64_t)get_prime(start);
        uint64_t prev = (uint64_t)get_prime(start - 1);
        uint64_t Gprev = cur - prev;                       /* gap preceding sublist */

        uint64_t next = (uint64_t)get_prime(start + 1);
        uint64_t Gfirst = next - cur;                      /* first gap inside sublist */

        /* Expand the current sublist */
        idx_t j = start + 1;   /* index of the candidate next prime */
        uint64_t prevPrime = cur;              /* last prime included in sublist */

        while (1) {
            uint64_t currPrime = (uint64_t)get_prime(j);
            uint64_t diff = currPrime - prevPrime;      /* gap to next prime */

            if (diff <= Gfirst && diff < Gprev) {
                prevPrime = currPrime;  /* include this prime in the sublist */
                j++;                    /* candidate next prime */
                continue;
            }
            break;                       /* gap violates condition: end sublist */
        }

        start = j;        /* next sublist starts at this prime index */
        count++;          /* one more sublist found */
    }

    return (seq_t)get_prime(start);
}
