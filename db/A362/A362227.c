/*
 * A362227  a(n) = Product_{k=1..w(n)} p(k)^(S(n,k)-1), where set S(n,k) = row
 *          n of A272011 and w(n) = A000120(n) is the binary weight of n.
 * https://oeis.org/A362227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1}^{w(n)} prime(k)^{(position_k+1)} where the set
 *            bits of n are examined from most to least significant. The k‑th
 *            set bit (k=1 for the most significant 1) is at position
 *            position_k (0‑based from the LSB), and its contribution is
 *            prime(k) raised to (position_k+1). This yields the value...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 58, u64 n <= 238,
 *            u128 n <= 1018, big any n
 * Validated: standard, pass, 65 terms through n = 9887 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow.h"

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
    /* Empty product for n = 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    idx_t rank = 0;                 /* 1‑based index of the set bit */

    /* Scan bits from most significant to least significant */
    for (int pos = 63; pos >= 0; --pos) {
        if ((n >> pos) & ((idx_t)1)) {
            ++rank;                                 /* prime index */
            seq_t p   = (seq_t)get_prime(rank);     /* prime(k) */
            seq_t exp = (seq_t)(pos + 1);            /* exponent = position+1 */
            seq_t mult = ipow(p, exp);            /* p^exp */
            prod *= mult;
        }
    }
    return prod;
}
