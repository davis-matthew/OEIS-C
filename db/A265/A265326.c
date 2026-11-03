/*
 * A265326  n-th prime minus its binary reversal.
 * https://oeis.org/A265326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = prime(n) - binary_reverse(prime(n)) (OEIS A265326)
 * Range:     i8 n <= 31, i16 n <= 3632, i32 n <= 23000, i64 n <= 23000,
 *            i128 n <= 23000, big any n
 * Validated: standard, pass, 75 terms through n = 9620 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the n-th prime via get_prime, then reverse its
   binary representation using bit manipulation.  All arithmetic is done
   in seq_t. */

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

static inline seq_t reverse_binary(seq_t v)
{
    seq_t rev = 0;
    /* determine bit length of v */
    unsigned bitlen = 0;
    seq_t tmp = v;
    while (tmp > 0) {
        tmp >>= 1;
        ++bitlen;
    }
    /* reverse bits */
    for (unsigned i = 0; i < bitlen; ++i) {
        rev <<= 1;
        rev |= v & 1;
        v >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* offset is 1; values for n<1 are undefined */

    /* get the n-th prime (1-indexed) */
    uint32_t p32 = get_prime(n);
    seq_t prime = (seq_t)p32;

    /* binary reverse of the prime */
    seq_t rev = reverse_binary(prime);

    /* a(n) = prime - reversed bits */
    return prime - rev;
}
