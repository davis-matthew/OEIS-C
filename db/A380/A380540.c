/*
 * A380540  a(n) is the multiplicative order of -7 modulo prime(n); a(4) = 0
 *          for completion.
 * https://oeis.org/A380540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = multiplicative order of (-7) modulo prime(n) (a(4) = 0
 *            because the order is undefined modulo 7)
 * Range:     u8 n <= 34, u16 n <= 4602, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8690 (2026-11-01)
 * Helpers:   build_primes_upto, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
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

static uint64_t *small_primes = NULL;
static size_t small_prime_cnt = 0;

static void init_small_primes(void) {
    if (small_primes) return;
    /* 400 is > sqrt(104729) so all needed trial factors are covered. */
    build_primes_upto(400, &small_primes, &small_prime_cnt);
}

seq_t a(idx_t n) {
    init_small_primes();

    uint32_t p = get_prime(n);            /* nth prime, 1‑based, as uint32_t */
    if (p == 7)                           /* special case: order undefined */
        return (seq_t)0;

    /* (-7) mod p  ==  (2p-7) mod p  ==  p-7  (for p>7)  */
    seq_t base = (seq_t)((uint64_t)p * 2 - 7) % (seq_t)p;

    uint64_t ord = (uint64_t)p - 1;       /* φ(p)  */
    uint64_t temp = ord;
    uint64_t factors[32];
    size_t factor_cnt = 0;

    /* factor ord using the small primes table */
    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint64_t q = small_primes[i];
        if (q * q > temp) break;
        if (temp % q == 0) {
            while (temp % q == 0)
                temp /= q;
            factors[factor_cnt++] = q;
        }
    }
    if (temp > 1)
        factors[factor_cnt++] = temp;      /* remaining prime factor */

    /* reduce the order by removing prime factors whenever possible   */
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t q = factors[i];
        if (ord % q == 0) {
            uint64_t candidate;
            while (ord % q == 0) {
                candidate = ord / q;
                if (powmod(base, candidate, p) != (seq_t)1)
                    break;
                ord = candidate;
            }
        }
    }

    return (seq_t)ord;
}
