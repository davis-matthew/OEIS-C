/*
 * A244557  Numbers obtained by concatenating the squares of the digits of
 *          prime(n).
 * https://oeis.org/A244557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat_of_squares_of_digits(prime(n)) a(n) =
 *            A048385(A000040(n))
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 960 (2026-11-01)
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

/* Helper: compute concatenation of digit squares for a prime */
static seq_t compute_prime_digit_squares(uint32_t p)
{
    /* Store digits in reverse for easy MSD processing */
    int digits[20];
    int cnt = 0;
    while (p > 0) {
        digits[cnt++] = p % 10;
        p /= 10;
    }

    seq_t result = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        uint32_t d = (uint32_t)digits[i];
        seq_t sq = (seq_t)d * d;          /* d² */
        seq_t mul = (sq < (seq_t)10) ? (seq_t)10 : (seq_t)100; /* shift for 1 or 2 decimal digits */
        result = result * mul + sq;
    }
    return result;
}

/* OEIS function definition */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n < 1, return 0 as a safe default */
        return 0;
    uint32_t p = get_prime(n);  /* p = prime(n) */
    return compute_prime_digit_squares(p);
}
