/*
 * A358225  Numbers k such that A276086(k) mod k is an odd number, where
 *          A276086 is the primorial base exp-function.
 * https://oeis.org/A358225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k such that A276086(k) mod k is
 *            odd, where A276086(k) = Π p_i^{d_i} with k expressed in
 *            primorial base (mixed–radix) using primes
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Compute (base^exp) mod mod with 64‑bit arithmetic */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (uint64_t)(((__uint128_t)result * b) % mod);
        b = (uint64_t)(((__uint128_t)b * b) % mod);
        exp >>= 1;
    }
    return result;
}

/* Return true iff k satisfies A276086(k) mod k is odd */
static bool parity_ok(idx_t k)
{
    if (k <= 1) return false;           /* k starts at 2 in the sequence */

    uint64_t rem = 1 % (uint64_t)k;     /* running product modulo k */
    uint64_t temp = (uint64_t)k;        /* remaining part of k to decompose */
    idx_t i = 0;                        /* digit index (prime_1 = 2) */

    while (temp > 0) {
        uint64_t prime = (uint64_t)get_prime(i + 1);
        uint64_t d = temp % prime;      /* Cantor digit */

        uint64_t powv = powmod_uint64(prime, d, k);
        rem = (uint64_t)(((__uint128_t)rem * powv) % k);

        temp /= prime;
        ++i;
    }

    return (rem & 1U) == 1U;            /* remainder odd? */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;            /* number of qualifying numbers found */
    idx_t candidate = 2;        /* start from 2 */

    while (1) {
        if (parity_ok(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
