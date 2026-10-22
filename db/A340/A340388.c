/*
 * A340388  Let n = p_1*p_2*...*p_k be the prime factorization of n, with the
 *          primes sorted in descending order. Then a(n) = 5^(p_1 - 1)*13^(p_2
 *          - 1)*17^(p_3 - 1)*...*A002144(k)^(p_k - 1).
 * https://oeis.org/A340388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1}^{k} A002144(i)^(p_i - 1) where p_i are the prime
 *            factors of n (with multiplicity) sorted in descending order
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* First 64 primes congruent to 1 modulo 4.  */
static const seq_t base1mod4[64] = {
    5,13,17,29,37,41,53,61,73,89,
    97,101,109,113,137,149,157,173,181,193,
    197,229,233,241,257,277,281,293,313,317,
    337,349,353,389,397,409,421,433,449,457,
    461,509,521,541,557,569,577,593,617,613,
    641,653,661,673,677,691,709,733,757,761,
    773,797,809,821
};

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1.  */
    if (n <= 1) return 1;

    /* Factor n.  */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t np = 0;
    factor_u64(temp, primes, exps, &np);

    /* Expand the factor list (with multiplicity).  */
    size_t tot = 0;
    for (size_t i = 0; i < np; ++i) tot += (size_t)exps[i];
    if (tot == 0) return 1;           /* should not happen for n > 1 */

    seq_t factors[64];
    size_t pos = 0;
    for (size_t i = 0; i < np; ++i) {
        for (size_t j = 0; j < (size_t)exps[i]; ++j) {
            factors[pos++] = primes[i];
        }
    }

    /* Sort the factors in descending order.  */
    for (size_t i = 1; i < tot; ++i) {
        seq_t key = factors[i];
        size_t j = i;
        while (j > 0 && factors[j-1] < key) {
            factors[j] = factors[j-1];
            --j;
        }
        factors[j] = key;
    }

    /* Compute the product.  */
    seq_t result = 1;
    for (size_t i = 0; i < tot; ++i) {
        seq_t p = factors[i];
        seq_t base = base1mod4[i];
        seq_t exp = p - 1;
        seq_t part = big_pow(base, exp);
        result *= part;       /* safe for values that fit in seq_t */
    }

    return result;
}
