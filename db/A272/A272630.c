/*
 * A272630  a(n) = binomial(3*prime(n), prime(n)) - 3*binomial(2*prime(n),
 *          prime(n)) + 3.
 * https://oeis.org/A272630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(3*prime(n), prime(n)) - 3*binomial(2*prime(n),
 *            prime(n)) + 3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 191 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

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

/* Compute binomial(n,k) exactly using a multiplicative algorithm.
   The result fits in seq_t. */
static seq_t binomial_exact(idx_t n, idx_t k)
{
    if (k > n) return 0u;
    if (k > n - k) k = n - k;          /* use symmetry */

    seq_t result = 1u;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* numerator term (n - k + i) and denominator term i */
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        /* cancel common factors between num/den and result */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(result, den);
        result /= g;
        den /= g;

        /* exact division: result is guaranteed to be a multiple of den */
        result = (result * num) / den;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0u) return 0u;           /* undefined for n < 1 */

    /* n-th prime (using 1‑based indexing) */
    idx_t p = get_prime(n);

    seq_t bin1 = binomial_exact(3u * p, p);  /* binomial(3p , p) */
    seq_t bin2 = binomial_exact(2u * p, p);  /* binomial(2p , p) */

    return bin1 - 3u * bin2 + 3u;
}
