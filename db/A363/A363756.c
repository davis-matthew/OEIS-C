/*
 * A363756  Total number of distinct ways the divisors of n can be the product
 *          of other nontrivial divisors of n.
 * https://oeis.org/A363756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d>1} (A001055(d)-1) A001055(d) = number of
 *            unordered factorizations of d into integers > 1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   part_factorisations
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/part_factorisations.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t N = (uint64_t)n;

    /* Factorise N.  For the size limits of the problem a simple trial division
       is sufficient and avoids any external dependency. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    uint64_t temp = N;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Generate all divisors of N.  We keep them in a local array of
       fixed maximum size.  The number of divisors of a 64‑bit integer
       does not exceed 103680, so 128 slots are already safe for the
       intended input sizes.  A larger size can be used if necessary. */
    uint64_t divs[128];
    size_t dcnt = 1;          /* start with divisor 1 */
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        size_t  old_dcnt = dcnt;

        uint64_t mult = 1;
        for (uint64_t e = 1; e <= exp; ++e) {
            mult *= p;
            for (size_t j = 0; j < old_dcnt; ++j) {
                divs[dcnt++] = divs[j] * mult;
            }
        }
    }

    /* Sum (A001055(d)-1) over divisors d>1. */
    seq_t total = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d > 1) {
            seq_t pcount = part_factorisations(d, 2);  /* A001055(d) */
            total += (pcount - 1);                    /* exclude the trivial factorisation */
        }
    }

    return total;
}
