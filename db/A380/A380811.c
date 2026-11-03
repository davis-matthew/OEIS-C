/*
 * A380811  Smallest composite number divisible by prime(n) which shares at
 *          least one decimal digit with prime(n).
 * https://oeis.org/A380811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1927, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9432 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A380811:  a(n) = smallest composite number divisible by
 * prime(n) that shares at least one decimal digit with prime(n).
 * The algorithm checks successive multiples of the prime until a share
 * of digits is found. */

#include <stdbool.h>
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

/* Additionally, the following helper utilities are available but not
 * required for this implementation: digit_counts, build_primes, etc. */

seq_t a(idx_t n)
{
    /* Return 0 for n < 1 (out of OEIS offset).  The specification
     * allows arbitrary behaviour below the offset. */
    if (n < 1)
        return 0;

    /* Obtain the n-th prime.  The helper function is provided by the
     * build.  It may allocate a cache internally, which is fine. */
    uint64_t p = get_prime(n);

    /* Pre‑compute which decimal digits appear in the prime. */
    bool p_digits[10] = { false };
    uint64_t tmp = p;
    do {
        p_digits[tmp % 10] = true;
        tmp /= 10;
    } while (tmp > 0);

    /* Check successive multiples p*k, starting with k = 2.  Since k≥2,
     * the product is guaranteed to be composite.  We stop at the first
     * multiple that shares any digit with the prime. */
    for (idx_t k = 2;; ++k) {
        seq_t m = (seq_t)p * (seq_t)k;

        /* Test whether m shares any digit with the prime. */
        seq_t mm = m;
        bool shares = false;
        do {
            uint8_t digit = (uint8_t)(mm % 10);
            if (p_digits[digit]) {
                shares = true;
                break;
            }
            mm /= 10;
        } while (mm > 0);

        if (shares)
            return m;
    }
}
