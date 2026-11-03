/*
 * A377281  Difference between the n-th prime and the next prime-power
 *          (exclusive).
 * https://oeis.org/A377281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A345531(n) - prime(n) (next prime power > prime(n) minus
 *            the prime)
 * Range:     u8 n <= 31, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"

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
    /* obtain the n-th prime and the (n+1)-th prime (next prime) */
    uint32_t p0 = get_prime(n);          /* nth prime */
    uint32_t p1 = get_prime(n + 1);      /* next prime */

    /* start with the difference to the next prime */
    seq_t best = (seq_t)p1 - (seq_t)p0;

    /* determine the largest exponent e such that 2^e > p0 */
    int e_max = 1;                       /* e such that 2^e > p0 */
    for (seq_t pow = 2; pow <= (seq_t)p0; pow <<= 1)
        ++e_max;

    /* search for composite prime powers that are closer */
    for (int e = 2; e <= e_max; ++e) {
        /* iterate over prime bases q and find smallest q^e > p0 */
        for (uint64_t q = 2; ; ++q) {
            if (!is_prime_seq(q))
                continue;

            /* compute q^e (loop instead of pow to stay in integer domain) */
            seq_t val = 1;
            for (int i = 0; i < e; ++i) {
                val *= (seq_t)q;
                if (val > (seq_t)p0 && val - (seq_t)p0 < best)
                    break;
            }
            if (val > (seq_t)p0) {
                seq_t diff = val - (seq_t)p0;
                if (diff < best)
                    best = diff;
                break;               /* larger primes yield larger val */
            }
        }
    }

    return best;
}
