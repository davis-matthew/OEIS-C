/*
 * A255173  Numbers k such that 1+prime(k) and 1+prime(k+1) are the product of
 *          the same number of primes.
 * https://oeis.org/A255173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   bigomega_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that 1+prime(k) and 1+prime(k+1) have the same number
   of prime factors (counted with multiplicity). */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/bigomega_seq.h"
#include "../utils/mem.h"

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
    /* offset 1: a(1) is the first such k.  Return the nth stored term. */
    if (n == 0) return 0; /* prevent invalid access, not part of the sequence */

    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static idx_t last_k = 0;

    /* Ensure we have at least n terms computed.  The search moves only forward,
       so repeated calls reuse previous work. */
    while (terms_len < (size_t)n) {
        ++last_k;

        seq_t pk  = (seq_t)get_prime(last_k);
        seq_t pk1 = (seq_t)get_prime(last_k + 1);

        seq_t c1 = bigomega_seq(pk + (seq_t)1);
        seq_t c2 = bigomega_seq(pk1 + (seq_t)1);

        if (c1 == c2) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = last_k;
        }
    }

    return terms[n - 1];
}
