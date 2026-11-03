/*
 * A358170  Heinz number of the partial sums of the n-th composition in
 *          standard order (A066099).
 * https://oeis.org/A358170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{s∈partial sums of the n-th composition in standard
 *            order} prime(s) (Heinz number of the partition given by the
 *            partial sums)
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 0)            /* empty composition → Heinz number 1 */
        return (seq_t)1;

    /* collect 1‑bit positions (1‑based, little‑endian) */
    uint64_t pos[65];
    uint64_t cnt = 0;
    idx_t k = n;
    for (uint64_t i = 1; k; ++i) {
        if (k & 1ULL)
            pos[cnt++] = i;   /* position i is set */
        k >>= 1;
    }

    /* differences of [0] ∪ pos[] give the parts of the composition */
    uint64_t part[65];
    for (uint64_t i = 0; i < cnt; ++i) {
        uint64_t prev = (i == 0) ? 0 : pos[i - 1];
        part[i] = pos[i] - prev;
    }

    /* compute partial sums in reverse order (standard Order) */
    seq_t prod = (seq_t)1;
    seq_t sum  = (seq_t)0;
    for (size_t i = cnt; i > 0; ) {
        --i;
        sum += (seq_t)part[i];
        prod *= (seq_t)get_prime((idx_t)sum);
    }

    return prod;
}
