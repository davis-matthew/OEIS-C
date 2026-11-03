/*
 * A273258  Write the distinct prime divisors p of n in the (PrimePi(p) -
 *          1)-th place, ignoring multiplicity. Decode the resulting number
 *          after first reversing the code, ignoring any leading zeros.
 * https://oeis.org/A273258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = A019565(A030101(A087207(n)))
 * Range:     u8 n <= 77, u16 n <= 1109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9084 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: encode n by writing "1" in the (PrimePi(p)-1)-th place for each distinct p|n,
   reverse the binary string, drop leading zeros, and decode as the product of primes
   whose indices correspond to the remaining "1" positions. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

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

seq_t a(idx_t n) {
    if (n == 1) return 1;          /* f(1) = "0" -> result 1 */

    /* factor n into distinct primes */
    idx_t temp = n;
    const idx_t MAX_FACTORS = 32;
    idx_t idx[MAX_FACTORS];
    idx_t cnt = 0;
    idx_t max_index = 0;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* distinct prime p found */
            while (temp % p == 0) temp /= p;
            idx_t pi_p = (idx_t)prime_pi(p);   /* pi(p) */
            idx_t i = pi_p - 1;                  /* 0‑based index */
            idx[cnt++] = i;
            if (i > max_index) max_index = i;
        }
    }
    if (temp > 1) {            /* last prime factor > sqrt */
        idx_t pi_p = (idx_t)prime_pi(temp);
        idx_t i = pi_p - 1;
        idx[cnt++] = i;
        if (i > max_index) max_index = i;
    }

    /* decode by reversing indices and multiplying the corresponding primes */
    seq_t result = 1;
    for (idx_t k = 0; k < cnt; ++k) {
        idx_t i = idx[k];
        idx_t j = max_index - i;                  /* reversed position */
        seq_t prime_j = (seq_t)get_prime((idx_t)j + 1);  /* j+1-th prime, 1‑based */
        result = (seq_t)((unsigned __int128)result *
                          (unsigned __int128)prime_j);
    }
    return result;
}
