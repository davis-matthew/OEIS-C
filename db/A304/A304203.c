/*
 * A304203  If n = Product (p_j^k_j) then a(n) = Product (p_j^prime(k_j)).
 * https://oeis.org/A304203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^k‖n} p^prime(k) where prime(k) is the k‑th prime
 *            number (prime(1)=2)
 * Range:     u8 n <= 16, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 988 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

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
    if (n <= 1) return (seq_t)1;   /* a(1)=1 */

    uint64_t n64 = (uint64_t)n;

    /* Small buffers are enough for n up to huge values.
       The maximum number of distinct prime factors of a 64‑bit
       integer is < 15. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64(n64, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p   = (seq_t)primes[i];
        seq_t exp = (seq_t)exps[i];

        /* prime(exp) → the exp‑th prime number */
        seq_t prime_exp = (seq_t)get_prime((idx_t)exp);

        /* p^prime(exp) */
        seq_t pow_val = ipow(p, prime_exp);

        result *= pow_val;
    }

    return result;
}
