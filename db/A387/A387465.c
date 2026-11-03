/*
 * A387465  Irregular triangle T(n,k), n >= 0, 0 <= k < 2^(n-1), where T(n,k)
 *          = Product_{j=0..n-1} prime(n-j)^((j+1)*d_j), where d_j is the bit
 *          with digit weight 2^j in the binary expansion of 2^(n-1)+k.
 * https://oeis.org/A387465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = ∏_{j=0}^{r-1, bit_j(n)=1} prime(r-j)^(j+1) where r =
 *            ⌊log₂ n⌋ + 1 (for n > 0), and a(0) = 1. This follows directly
 *            from the triangle definition: T(r,k) = ∏_{j=0}^{r-1}
 *            prime(r-j)^{(j+1)*d_j} with d_j the j‑th binary digit of n =
 *            2^(r-1)+k
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 46, u64 n <= 188, u128 n <= 766,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 9104 (2026-11-01)
 * Helpers:   ilog2, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ilog2.h"
#include "../utils/ipow.h"

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
    if (n == 0)
        return (seq_t)1;

    /* r = floor(log2(n)) + 1   (since n > 0) */
    idx_t r = ilog2(n) + 1;

    seq_t result = (seq_t)1;

    for (idx_t j = 0; j < r; ++j)
    {
        if ((n >> j) & (idx_t)1)          /* bit j of n is set? */
        {
            idx_t prime_idx = r - j;       /* prime(r-j) */
            seq_t prime_val = (seq_t)get_prime(prime_idx);  /* kth prime (k≥1) */

            /* exponent = j+1 (small, fits in seq_t) */
            seq_t pow_val = ipow(prime_val, (seq_t)(j + 1));

            /* multiply into the running product */
            result *= pow_val;               /* works for builtin and big‑integer types */
        }
    }

    return result;
}
