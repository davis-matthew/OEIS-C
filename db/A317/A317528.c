/*
 * A317528  Expansion of Sum_{k>=1} mu(k)^2*x^k/(1 + x^k), where mu() is the
 *          Moebius function (A008683).
 * https://oeis.org/A317528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 2^{k} if n is odd (k = number of distinct odd prime
 *            factors), 0 if 2 divides n exactly once, -2^{k+1} if 2 divides n
 *            at least twice. This follows from a(2)=0, a(2^e)=-2 (e>1) and
 *            a(p^e)=2 for odd primes
 * Range:     i8 n <= 9832, i16 n <= 9832, i32 n <= 9832, i64 n <= 9832,
 *            i128 n <= 9832, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

#define MAX_PRIMES 64

seq_t a(idx_t n) {
    if (n == (idx_t)1)
        return (seq_t)1;

    idx_t tmp = n;
    uint64_t exp2 = 0;
    while ((tmp & 1) == 0) {
        exp2++;
        tmp >>= 1;
    }

    uint64_t odd_part = tmp;
    uint64_t primes[MAX_PRIMES];
    uint64_t exps[MAX_PRIMES];
    size_t cnt = 0;
    if (odd_part > 1)
        factor_u64(odd_part, primes, exps, &cnt);
    else
        cnt = 0;                  /* no odd prime factors */

    uint64_t k = cnt;          /* number of distinct odd primes */

    if (exp2 == 0) {
        /* odd n -> 2^k */
        return ((seq_t)1 << (int)k);
    }
    if (exp2 == 1) {
        /* exactly one factor 2 -> 0 */
        return (seq_t)0;
    }
    /* exp2 >= 2 -> -2^{k+1} */
    return -( (seq_t)1 << (int)(k + 1) );
}
