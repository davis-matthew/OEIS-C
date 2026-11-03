/*
 * A254939  a(n) = (A099795(n)^-1 mod p)*A099795(n), where p = prime(n).
 * https://oeis.org/A254939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (L^-1 mod p) * L where L = lcm(1..p-1) and p = the n-th
 *            prime
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/powmod_u64.h"

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
    /* Obtain the n-th prime. */
    uint32_t p = get_prime(n);

    /* Compute L = lcm(1..p-1) as product of prime powers. */
    seq_t L = 1;
    /* Iterate over all primes q <= p-1. */
    for (idx_t k = 1;; ++k) {
        uint32_t q = get_prime(k);
        if (q >= p) break;           /* stop when q reaches p */
        uint32_t limit = p - 1;
        uint64_t power = q;
        while (power * (uint64_t)q <= (uint64_t)limit) {
            power *= q;              /* find highest power of q <= limit */
        }
        L = L * (seq_t)power;        /* multiply into L */
    }

    /* Compute L modulo p. */
    uint64_t Lmodp = (uint64_t)(L % (seq_t)p);

    /* Compute modular inverse of Lmodp modulo p (p is prime). */
    uint64_t inv = powmod_u64(Lmodp, (uint64_t)p - 2, p);

    /* Result is L * inv. */
    seq_t result = L * (seq_t)inv;
    return result;
}
