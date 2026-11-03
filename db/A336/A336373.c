/*
 * A336373  Primes prime(k) such that gcd(k, prime(k)+prime(k-1)) > 1.
 * https://oeis.org/A336373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A336373
   a(n) = prime(n) such that gcd(n, prime(n)+prime(n-1)) > 1
   Implementation uses incremental generation of primes via get_prime().
   The sequence values are stored in a static dynamic array so that
   subsequent calls to a() can resume efficiently, independent of call
   order.
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static storage for the sequence values and state */
    static seq_t *seq      = NULL;   /* array of qualifying primes */
    static size_t seq_cap  = 0;      /* allocated capacity */
    static size_t seq_len  = 0;      /* number of values stored */
    static idx_t  last_k   = 1;      /* last examined prime index */

    /* Return cached value if we already have it */
    if (n <= seq_len)
        return seq[n - 1];

    /* Grow the array when necessary */
    if (n > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
        seq   = tmp;
        seq_cap = new_cap;
    }

    /* Scan successive prime indices until we have n terms */
    while (seq_len < n) {
        ++last_k;                          /* next index k */

        /* Pull kth and (k‑1)th primes (use uint32_t from get_prime()) */
        seq_t pk     = (seq_t)get_prime(last_k);
        seq_t pk_prev = (seq_t)get_prime(last_k - 1);

        /* Compute gcd(k, pk + pk_prev) */
        seq_t sum = pk + pk_prev;
        seq_t g   = gcd((seq_t)last_k, sum);

        /* If gcd > 1, store the prime */
        if (g > (seq_t)1) {
            seq[seq_len++] = pk;
        }
    }

    return seq[n - 1];
}
