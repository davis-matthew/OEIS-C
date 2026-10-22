/*
 * A352553  a(n) = number of modules with n elements over the ring of integers
 *          in the real quadratic field of discriminant 13.
 * https://oeis.org/A352553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   legendre_13_fundamental
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of modules of size n over the ring of integers of Q(√13).  
   The algorithm is multiplicative over the prime factorization of n.  
   For an inert prime p (including 2) the exponent e must be even and
   contributes a factor p(e/2), where p(k) is the k‑th partition number.  
   For a split prime p the contribution is Σₖ₌₀ᵉ p(k)·p(e‑k).  
   For the ramified prime 13 the contribution is p(e).  
   The product of these contributions gives a(n).  */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/legendre_13_fundamental.h"

/* Partition numbers p(0)…p(13).  These are sufficiently large for n≤10000. */
static const seq_t part[14] = {
    1ULL,     /* p(0) */
    1ULL,     /* p(1) */
    2ULL,     /* p(2) */
    3ULL,     /* p(3) */
    5ULL,     /* p(4) */
    7ULL,     /* p(5) */
    11ULL,    /* p(6) */
    15ULL,    /* p(7) */
    22ULL,    /* p(8) */
    30ULL,    /* p(9) */
    42ULL,    /* p(10) */
    56ULL,    /* p(11) */
    77ULL,    /* p(12) */
    101ULL    /* p(13) */
};

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;              /* trivial module */

    seq_t result = (seq_t)1;
    idx_t temp   = n;

    /* Handle the inert prime 2 separately. */
    unsigned int e2 = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++e2;
    }
    if (e2 & 1)                     /* odd exponent → impossible */
        return (seq_t)0;
    if (e2 > 0) result *= part[e2 >> 1];

    /* Factor remaining odd primes. */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            seq_t factor;
            if (p == 13) {                 /* ramified prime */
                factor = part[e];
            } else {
                int leg = legendre_13_fundamental((int)p);
                if (leg == 1) {             /* split prime */
                    seq_t sum = (seq_t)0;
                    for (unsigned int k = 0; k <= e; ++k)
                        sum += part[k] * part[e - k];
                    factor = sum;
                } else {                     /* inert prime */
                    if (e & 1) return (seq_t)0;
                    factor = part[e >> 1];
                }
            }
            result *= factor;
            if (result == (seq_t)0) return (seq_t)0;
        }
    }

    /* Handle a remaining prime factor > sqrt(original n). */
    if (temp > 1) {
        idx_t p = temp;
        unsigned int e = 1;                 /* exponent is 1 */

        seq_t factor;
        if (p == 13) {
            factor = part[1];
        } else {
            int leg = legendre_13_fundamental((int)p);
            if (leg == 1) {                 /* split prime */
                factor = part[0] * part[1] + part[1] * part[0];
            } else {                         /* inert prime */
                if (e & 1) return (seq_t)0;
                factor = part[e >> 1];     /* e==1 → part[0] = 1 */
            }
        }
        result *= factor;
    }

    return result;
}
