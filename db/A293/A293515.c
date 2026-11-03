/*
 * A293515  a(n) = Product_{d^k|n, d>1, k>1} prime(A286561(n,d)-1), where
 *          A286561(n,d) gives the highest exponent of d dividing n.
 * https://oeis.org/A293515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d^k|n, d>1, k>1} prime(A286561(n,d)-1)
 * Range:     u8 n <= 255, u16 n <= 5183, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 133 terms through n = 9876 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A286561(n,d) gives the largest exponent e such that d^e divides n */
/* The product is over all integers d>1 with d^2|n, and each factor is the prime with index (e-1). */

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

seq_t a(idx_t n) {
    seq_t result = 1;

    /* For n <= 1 the product is empty → 1. */
    /* Loop over all candidates d with d^2 ≤ n. */
    for (idx_t d = 2; d <= n / d; ++d) {
        /* d must divide n, and n/d must also be divisible by d for d^2|n. */
        if (n % d == 0) {
            idx_t n_div_d = n / d;
            if (n_div_d % d == 0) {
                /* Compute the maximal exponent e such that d^e | n. */
                idx_t temp = n;
                idx_t e = 0;
                while (temp % d == 0) {
                    temp /= d;
                    ++e;
                }
                /* e >= 2 because d^2 divides n. */
                if (e >= 2) {
                    /* Multiply by the prime with index (e-1). */
                    result *= (seq_t)get_prime(e - 1);
                }
            }
        }
    }
    return result;
}
