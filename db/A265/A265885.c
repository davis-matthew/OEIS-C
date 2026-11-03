/*
 * A265885  a(n) = n IMPL prime(n), where IMPL is the bitwise logical
 *          implication.
 * https://oeis.org/A265885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n IMPL prime(n) (bitwise logical implication)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8028 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/bit_length.h"

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

static inline seq_t
mask_for_len(unsigned int len)
{
    unsigned int bits = sizeof(seq_t) * 8;
    if (len == 0)
        return (seq_t)0;
    if (len >= bits)
        return ~((seq_t)0);           /* all bits set when mask would be full width */
    return (((seq_t)1 << len) - 1);
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; n==0 shouldn't happen, but guard anyway */
        return 0;

    uint32_t p32 = get_prime(n);     /* prime(n) is always < 2^32 for n <= 10⁵ */
    seq_t p   = (seq_t)p32;
    unsigned int len = bit_length(p);

    seq_t mask = mask_for_len(len);

    seq_t nn = (seq_t)n;
    seq_t res = ((~nn) | p) & mask;
    return res;
}
