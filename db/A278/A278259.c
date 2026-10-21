/*
 * A278259  Least number with the prime signature of the n-th quarter-square,
 *          a(1) = 0.
 * https://oeis.org/A278259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n) = A046523(A002620(n))
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>            /* for uint64_t */
#include "../utils/factor_u64.h"  /* factor_u64: factorize uint64_t numbers */
#include "../utils/seq_pow.h"    /* seq_pow: integer exponentiation in seq_t */

static const seq_t SMALL_PRIMES[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* special case from definition */

    /* Compute the n-th quarter‑square: A002620(n) = floor(n^2/4) */
    seq_t q = ((seq_t)n * (seq_t)n) / 4;      /* integer division is floor */

    /* Factor q.  For n up to about 10⁴, q fits in 64 bit. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt;

    if (q == 0) {                 /* only possible for n==0, safe guard */
        return 0;
    }

    uint64_t q64 = (uint64_t)q;   /* q fits in 64 bit for the intended limits */
    factor_u64(q64, primes, exps, &pcnt);

    /* Sort exponents in non‑increasing order (larger exponents to smaller primes). */
    for (size_t i = 0; i < pcnt; ++i) {
        for (size_t j = i + 1; j < pcnt; ++j) {
            if (exps[i] < exps[j]) {
                uint64_t tmp = exps[i];
                exps[i] = exps[j];
                exps[j] = tmp;
            }
        }
    }

    /* Build the minimal number using the smallest primes. */
    seq_t result = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t term = seq_pow((seq_t)SMALL_PRIMES[i], (idx_t)exps[i]);
        result *= term;
    }

    return result;
}
