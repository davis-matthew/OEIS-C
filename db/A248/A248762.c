/*
 * A248762  Greatest cube that divides n!.
 * https://oeis.org/A248762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product over primE p ≤ n of p^(3·⌊v_p(n!)/3⌋), where
 *            v_p(n!) is the exponent of prime p in n!. This is the largest
 *            cube dividing n!
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 19, u64 n <= 27, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases early */
    if (n < 2)
        return (seq_t)1;

    /* Static cache of primes and current limit */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    /* Extend prime cache if necessary */
    if (n > prime_limit) {
        build_primes_upto(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    seq_t result = (seq_t)1;

    /* For each prime p ≤ n, determine exponent in n! */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        idx_t div = n / p;
        if (div == 0) /* primes are sorted ascending, break early */
            break;

        idx_t ep = 0;
        while (div) {
            ep += div;
            div /= p;
        }

        idx_t cube_exp = (ep / 3) * 3;
        if (cube_exp == 0)
            continue;

        seq_t power = big_pow((seq_t)p, (seq_t)cube_exp);
        result = result * power;
    }

    return result;
}
