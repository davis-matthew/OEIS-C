/*
 * A244797  Number of moduli m such that (prime(n) mod m) = 2.
 * https://oeis.org/A244797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (prime(n)-2) minus 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 984 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

/* Count of divisors of a positive integer x (x ≥ 1). */
static seq_t divisor_count(uint64_t x) {
    if (x <= 1) return 1;          /* τ(1) = 1 */
    seq_t cnt = 1;
    uint64_t tmp = x;
    for (uint64_t d = 3; d * d <= tmp; d += 2) {
        if (tmp % d == 0) {
            int exp = 0;
            while (tmp % d == 0) {
                tmp /= d;
                exp++;
            }
            cnt *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        cnt *= (seq_t)2;
    }
    return cnt;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* out of offset */
    uint32_t prime = get_prime(n);
    if (prime == 2) return 0;        /* p-2 = 0 yields no valid moduli */
    uint64_t m = (uint64_t)prime - 2ULL;  /* p-2 > 0 and odd for p ≥ 3 */
    seq_t dcnt = divisor_count(m);
    return dcnt - 1;                /* subtract divisor 1 */
}
