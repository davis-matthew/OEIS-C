/*
 * A305458  In primorial base: a(n) is obtained by replacing each nonzero
 *          digit of n with its product with the nonzero digits at lower
 *          indices (See Comments for precise definition).
 * https://oeis.org/A305458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n)=∑_{i≥0} e_i·P_i where P_i=∏_{k=1..i}prime(k) and for the
 *            primorial‑base digits d_j of n: e_i = 0 if d_i = 0 e_i =
 *            (∏_{j≤i,d_j>0} d_j) mod prime(i+1) otherwise (the product is
 *            taken over all non‑zero digits up to the i‑th position)
 * Range:     u8 n <= 213, u16 n <= 2310, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 80 terms through n = 2303 (2026-11-01)
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 0;      /* accumulated value */
    seq_t weight = 1;      /* primorial weight P_i  (product of first i primes) */
    seq_t product = 1;     /* full product of all non‑zero digits so far */

    idx_t rest = n;        /* remaining part of n in primorial base */
    idx_t i = 0;           /* digit index (0‑based) */

    for (;; ++i)
    {
        uint32_t prime = get_prime(i + 1);          /* prime(i+1) */
        uint64_t digit = rest % (uint64_t)prime;    /* current digit d_i */
        rest /= (uint64_t)prime;                    /* shift right for next digit */

        seq_t ei = 0;
        if (digit != 0)
        {
            product = product * (seq_t)digit;          /* exact product so far */
            ei      = product % (seq_t)prime;         /* product mod current prime */
        }

        result += ei * weight;                     /* add contribution */
        weight *= (seq_t)prime;                    /* advance weight to next primorial */

        if (rest == 0) break;                      /* finished all digits */
    }

    return result;
}
