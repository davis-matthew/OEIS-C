/*
 * A381224  a(n) is the integer resulting from the concatenation of the unit
 *          digit of prime(n-1) to the digits of prime(n) without its own unit
 *          digit.
 * https://oeis.org/A381224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = concatenation of the unit digit of prime(n-1) with the
 *            digits of prime(n) without its own unit digit. a(1) = 0
 * Range:     u8 n <= 25, u16 n <= 1230, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9407 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/num_digits.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }

    /* prime indices are zero‑based for get_prime, but the sequence
       definition uses 1‑based primes: prime(1)=2, prime(2)=3, ... */
    uint32_t prev_prime = get_prime(n - 1);
    uint32_t curr_prime = get_prime(n);

    /* digit to prepend */
    seq_t d = (seq_t)(prev_prime % 10u);

    /* If the current prime has only one digit, the result is simply the
       preceding digit. */
    if (curr_prime < 10) {
        return d;
    }

    seq_t after = (seq_t)(curr_prime / 10);
    int len     = num_digits(after);    /* number of decimal digits of after */
    seq_t pow   = pow10(len);          /* 10^len */
    return d * pow + after;
}
