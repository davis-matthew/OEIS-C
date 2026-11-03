/*
 * A259559  Numbers n such that prime(n)-1 and prime(n+1)-1 have the same
 *          number of prime factors, including repeats.
 * https://oeis.org/A259559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   bigomega_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259559 – n such that prime(n)-1 and prime(n+1)-1 have
   the same number of prime factors (counting multiplicities).
   Computed by checking consecutive primes. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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
    static seq_t *terms = NULL;      /* array of found indices n */
    static size_t terms_len = 0;     /* number of terms stored   */
    static size_t terms_cap = 0;     /* capacity of the array    */
    static idx_t next_prime_index = 1; /* index of prime to test */

    /* Generate terms until we have the requested count. */
    while (terms_len < (size_t)n)
    {
        /* Get current pair of consecutive primes. */
        uint32_t p_i   = get_prime(next_prime_index);
        uint32_t p_ip1 = get_prime(next_prime_index + 1);

        /* Count prime factors of (p_i - 1) and (p_ip1 - 1). */
        seq_t omega1 = bigomega_seq((seq_t)p_i - (seq_t)1);
        seq_t omega2 = bigomega_seq((seq_t)p_ip1 - (seq_t)1);

        if (omega1 == omega2)
        {
            /* Grow the terms array if necessary. */
            if (terms_len == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 4;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) abort();           /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_prime_index;
        }

        /* Advance to check the next pair. */
        next_prime_index++;
    }

    return terms[n - 1];
}
