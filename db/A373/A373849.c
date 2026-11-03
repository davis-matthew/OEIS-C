/*
 * A373849  Difference between the primorial base exp-function and the
 *          arithmetic derivative.
 * https://oeis.org/A373849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 57, i32 n <= 412, i64 n <= 2306,
 *            i128 n <= 30030, big any n
 * Validated: standard, pass, 80 terms through n = 9780 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A276086(n) - A003415(n)
 * A276086: product of primes ^ (digits in primorial base of n)
 * A003415: arithmetic derivative of n
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

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
    /* Handle n = 0 separately */
    if (n == 0)
        return (seq_t)1;

    /* ---- Compute A276086(n) : primorial base exp-function ---- */
    seq_t product = 1;
    idx_t rem = n;
    idx_t prime_idx = 1;

    /* Store the primes and digits (maximum 64 digits suffices for n ≤ 10000) */
    struct { uint64_t p; int d; } digits[64];
    int pos = 0;

    while (rem > 0)
    {
        uint64_t p = get_prime(prime_idx);      /* i-th prime, 1‑based */
        int d = (int)(rem % p);                /* digit at this position */
        rem /= p;
        digits[pos].p = p;
        digits[pos].d = d;
        pos++;
        prime_idx++;
    }

    for (int i = 0; i < pos; ++i)
    {
        if (digits[i].d > 0)
            product *= ipow_i32((seq_t)digits[i].p, digits[i].d);
    }

    /* ---- Compute A003415(n) : arithmetic derivative of n ---- */
    seq_t derivative = 0;
    idx_t temp = n;
    prime_idx = 1;

    while (temp > 1)
    {
        uint64_t p = get_prime(prime_idx);
        /* stop if p² > remaining factor */
        if ((idx_t)p * (idx_t)p > temp)
            break;

        int exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            exp++;
        }
        if (exp > 0)
            derivative += (seq_t)(n / p) * (seq_t)exp;

        prime_idx++;
    }

    /* Remaining prime factor, if any */
    if (temp > 1)
        derivative += (seq_t)(n / temp);

    /* ---- Return the difference ---- */
    return product - derivative;
}
