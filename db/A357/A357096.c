/*
 * A357096  Least number whose set of decimal digits coincides with the set of
 *          decimal digits of prime(n).
 * https://oeis.org/A357096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the least positive integer whose set of decimal digits
 *            equals the set of decimal digits of prime(n)
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
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

/* Helper: compute minimal number from a digit‑set mask. */
static seq_t min_from_mask(unsigned int mask)
{
    uint8_t digits[10];
    int cnt = 0;
    for (int d = 0; d < 10; d++)
        if (mask & (1U << d))
            digits[cnt++] = d;

    /* The least digit is at digits[0] (ascending order). If it is 0,
       swap it with the first non‑zero digit to avoid leading zero. */
    if (cnt > 1 && digits[0] == 0) {
        int j = 1;
        while (j < cnt && digits[j] == 0) j++;
        if (j < cnt) {
            uint8_t temp = digits[0];
            digits[0] = digits[j];
            digits[j] = temp;
        }
    }

    seq_t val = 0;
    for (int i = 0; i < cnt; i++)
        val = val * 10 + digits[i];
    return val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based per OEIS definition */
    uint32_t p = get_prime(n);      /* prime(n) as uint32_t */
    unsigned int mask = 0;
    uint32_t tmp = p;
    while (tmp > 0) {
        mask |= 1U << (tmp % 10);
        tmp /= 10;
    }
    return min_from_mask(mask);
}
