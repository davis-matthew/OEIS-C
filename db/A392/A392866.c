/*
 * A392866  a(n) = A276086(n) mod A117366(A276086(n)), where A117366 gives the
 *          smallest prime greater than the largest prime dividing n, and
 *          A276086 is the primorial base exp-function.
 * https://oeis.org/A392866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30029, u16 n <= 30029, u32 n <= 30029, u64 n <= 30029,
 *            u128 n <= 30029, big any n
 * Validated: standard, pass, 118 terms through n = 9890 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A392866
 *  a(n) = A276086(n) mod A117366(A276086(n))
 *  where A276086(n) is the primorial‑base base‑product of n, and
 *  A117366(m) gives the smallest prime greater than the largest prime
 *  divisor of m.
 *
 *  Algorithm:  obtain the primorial‑base digits of n, noting the
 *  largest prime used.  The modulus is the next prime after that
 *  prime (or 2 if n = 0).  Compute the primorial‑base product
 *  modulo this modulus by multiplying each prime raised to its
 *  digit power, all reductions performed modulo the modulus.
 *  Complexity:  O(number of digits in the primorial expansion).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

/* Return the required term.  No external state is used. */
seq_t a(idx_t n)
{
    /* Special case n = 0:  A276086(0) = 1, largest prime divisor = none,
     * so A117366(1) = 2.  1 mod 2 = 1. */
    if (n == 0)
        return (seq_t)1;   /* 1 mod 2 == 1 */

    /* Find the largest prime index used in the primorial base
     * representation of n.  (i is 0‑based here, so get_prime(i+1)). */
    idx_t max_i = 0;
    idx_t t = n;
    idx_t i = 0;
    while (t > 0) {
        uint32_t prime = get_prime(i + 1);
        uint32_t digit = (uint32_t)(t % prime);
        if (digit != 0)
            max_i = i;      /* keep the last (hence largest) index */
        t /= prime;
        ++i;
    }

    /* Modulus is the next prime after the largest one used. */
    uint32_t mod = get_prime(max_i + 2);   /* max_i+1 is last used prime,
                                             so +1 gives next prime. */

    /* Compute the primorial‑base product modulo mod. */
    uint64_t result = 1ULL % mod;
    t = n;
    i = 0;
    while (t > 0) {
        uint32_t prime = get_prime(i + 1);
        uint32_t digit = (uint32_t)(t % prime);
        if (digit != 0) {
            uint64_t base = prime % mod;
            uint64_t powv = 1ULL % mod;
            uint32_t e = digit;
            while (e) {
                if (e & 1)
                    powv = (powv * base) % mod;
                e >>= 1;
                if (e)
                    base = (base * base) % mod;
            }
            result = (result * powv) % mod;
        }
        t /= prime;
        ++i;
    }

    return (seq_t)result;
}
