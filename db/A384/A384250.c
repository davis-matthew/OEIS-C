/*
 * A384250  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is an infinitary divisor of n is a powerful
 *          number.
 * https://oeis.org/A384250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p^e) where f(p^e) is the number of integers k
 *            (1 ≤ k ≤ p^e) whose greatest divisor that is an infinitary
 *            divisor of p^e is a powerful number. f(p^e) is computed by a
 *            direct enumeration of k
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation details:
    – The function `a_prime_power` enumerates all k from 1 to p^e.
    – It finds the exponent of p in k, then the largest exponent
      b that is a sub‑mask of e (b & ~e == 0).
    – If b is 0 or ≥ 2, k contributes to the count.
    – The final answer is the product of these counts for each prime
      power dividing n.
*/

#include <stddef.h>            /* for size_t */
#include <stdint.h>            /* for uint64_t */
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(n, primes, exps, &cnt) */

static seq_t a_prime_power(uint64_t p, idx_t e)
{
    /* Compute p^e as limit of the loop */
    idx_t limit = 1;
    for (idx_t i = 0; i < e; ++i)
        limit *= p;

    seq_t count = 0;
    for (idx_t k = 1; k <= limit; ++k) {
        /* exponent of p in k */
        idx_t t = k;
        idx_t exp = 0;
        while ((t % p) == 0) {
            t /= p;
            ++exp;
        }

        /* largest sub‑mask exponent b ≤ exp with b & ~e == 0 */
        idx_t b = exp;
        while ((b & ~e) != 0)
            --b;

        /* powerful if exponent 0 or ≥ 2 */
        if (b == 0 || b >= 2)
            ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    /* Handle n = 1 trivially */
    if (n <= 1)
        return (seq_t)1;

    /* Factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t contrib = a_prime_power(primes[i], exps[i]);
        result *= contrib;
    }
    return result;
}
