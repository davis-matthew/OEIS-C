/*
 * A277017  Number of maximal runs of 1-bits (in binary expansion of n) such
 *          that the length of run >= A000040(1 + the total number of zeros to
 *          the right of that run).
 * https://oeis.org/A277017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of maximal runs of 1-bits (in binary expansion of
 *            n) such that the length of run >= A000040(1 + the total number
 *            of zeros to the right of that run)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 137 terms through n = 8118 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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
    seq_t bits = n;          /* working copy of the binary representation */
    seq_t count = 0;         /* result to return */
    idx_t zeros = 0;         /* number of zeros seen in less significant bits */

    while (bits != (seq_t)0) {
        /* If the least significant bit is zero, record it and continue */
        if ((bits & (seq_t)1) == (seq_t)0) {
            zeros++;
            bits >>= 1;
        } else {
            /* Found the start of a maximal run of 1-bits */
            seq_t run_len = 0;
            while ((bits & (seq_t)1) != (seq_t)0) {
                run_len++;
                bits >>= 1;
            }
            /* zeros to the right of this run are the zeros seen so far */
            seq_t prime_needed = (seq_t)get_prime((idx_t)zeros + 1);
            if (run_len >= prime_needed)
                count++;
            /* no reset of zeros; the zeros counted already belong
               to less significant bits of the number */
        }
    }

    return count;
}
