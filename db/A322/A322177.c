/*
 * A322177  If n = Product (p_j^k_j) then a(n) = Sum (prime(p_j)^prime(k_j)).
 * https://oeis.org/A322177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p^k||n} prime(p)^prime(k)
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 9, etc.  (see OEIS A322177) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
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
    if (n <= 1)            /* a(1) = 0 */
        return 0;

    seq_t sum = 0;
    idx_t temp = n;

    /* trial division for small primes */
    for (idx_t p = 2; (unsigned __int128)p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp_cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp_cnt;
            }
            seq_t base = (seq_t)get_prime(p);      /* prime(p)   */
            seq_t exp  = (seq_t)get_prime(exp_cnt);/* prime(k)   */
            sum += ipow(base, exp);
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t base = (seq_t)get_prime(temp);         /* prime(p)   */
        seq_t exp  = (seq_t)get_prime(1);            /* prime(1)=2 */
        sum += ipow(base, exp);
    }

    return sum;
}
