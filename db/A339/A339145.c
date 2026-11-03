/*
 * A339145  a(n) is 1 if A196202(n) is a local maximum, -1 if A196202(n) is a
 *          local minimum and 0 otherwise.
 * https://oeis.org/A339145
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sign of local extremum of A196202(n) = 2^(p-1) mod p^2 (p
 *            is the n‑th prime). 1 for a local maximum, -1 for a local
 *            minimum, 0 otherwise. a(2) is the first term
 * Range:     i8 n <= 6, i16 n <= 42, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 79 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/powmod.h"

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
    /* Sequence offset is 2; values below are undefined. */
    if (n < 2)
        return 0;

    /* Retrieve the required primes.  All indices are valid for n >= 2. */
    uint32_t p_prev = get_prime(n - 1);
    uint32_t p_curr = get_prime(n);
    uint32_t p_next = get_prime(n + 1);

    /* Compute A196202 for each prime: 2^(p-1) mod p^2. */
    seq_t mod_prev = (seq_t)p_prev * p_prev;
    seq_t val_prev = powmod((seq_t)2, (uint64_t)p_prev - 1, mod_prev);

    seq_t mod_curr = (seq_t)p_curr * p_curr;
    seq_t val_curr = powmod((seq_t)2, (uint64_t)p_curr - 1, mod_curr);

    seq_t mod_next = (seq_t)p_next * p_next;
    seq_t val_next = powmod((seq_t)2, (uint64_t)p_next - 1, mod_next);

    /* Determine if val_curr is a strict local maximum or minimum. */
    if (val_curr > val_prev && val_curr > val_next)
        return 1;    /* local maximum */
    if (val_curr < val_prev && val_curr < val_next)
        return -1;   /* local minimum */
    return 0;        /* otherwise */
}
