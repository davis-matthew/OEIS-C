/*
 * A328622  In primorial base representation of n, multiply by 2 all other
 *          digits except the least significant, and reduce each such product
 *          modulo prime(k) (to get the new digit), where k > 1 is the
 *          position of the digit, then convert back to decimal.
 * https://oeis.org/A328622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A276085(A328618(A276086(n)))
 * Range:     u8 n <= 209, u16 n <= 30029, u32 n <= 30029, u64 n <= 30029,
 *            u128 n <= 30029, big any n
 * Validated: standard, pass, 98 terms through n = 9791 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Primorial‑base representation: double all digits except the least */
/* significant digit and reduce them modulo the corresponding prime, */
/* then recombine into a decimal value.) */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

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
    /* For n = 0 return 0 immediately. */
    if (n == 0)
        return 0;

    /* --- 1. Convert n to primorial base (digits < corresponding prime). -- */
    size_t cap = 8;                     /* initial capacity for digits array    */
    uint32_t *digits = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!digits)                        /* out‑of‑memory → fallback 0          */
        return 0;

    size_t len = 0;
    seq_t cur = (seq_t)n;               /* use seq_t to avoid overflow          */

    while (cur > 0) {
        if (len == cap) {
            cap <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(digits, cap * sizeof(uint32_t));
            if (!tmp) {
                free(digits);
                return 0;
            }
            digits = tmp;
        }
        /* prime for this digit position (1‑based). */
        uint64_t p = get_prime((idx_t)(len + 1));
        digits[len] = (uint32_t)(cur % p);
        cur /= p;
        ++len;
    }

    /* --- 2. Double all digits except the least significant one
           and reduce modulo the corresponding prime.                      */
    for (size_t i = 1; i < len; ++i) {   /* i==1 corresponds to position 2  */
        uint64_t p = get_prime((idx_t)(i + 1));
        digits[i] = (uint32_t)(((uint64_t)2 * digits[i]) % p);
    }

    /* --- 3. Reconstruct the result from the modified digits.           */
    seq_t res = 0;
    seq_t weight = 1;
    for (size_t i = 0; i < len; ++i) {
        uint64_t p = get_prime((idx_t)(i + 1));
        res += (seq_t)digits[i] * weight;
        weight *= (seq_t)p;
    }

    free(digits);
    return res;
}
