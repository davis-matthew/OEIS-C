/*
 * A295464  a(n) = (30*n)!*(5*n)!*(3*n)!/((15*n)!*(10*n)!*(7*n)!*(6*n)!).
 * https://oeis.org/A295464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((30*n)!*(5*n)!*(3*n)!)/((15*n)!*(10*n)!*(7*n)!*(6*n)!)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 3,
 *            big any n
 * Helpers:   _prime_exp_in_fact, build_primes, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* NULL, free() */
#include "../utils/build_primes.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/_prime_exp_in_fact.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Prepare prime list up to 30*n. */
    idx_t limit = 30 * n;
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(limit, &primes, &prime_cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = (idx_t)primes[i];

        /* Exponent of p in the product ratio. */
        idx_t e = _prime_exp_in_fact(30 * n, p)
                + _prime_exp_in_fact(5  * n, p)
                + _prime_exp_in_fact(3  * n, p)
                - _prime_exp_in_fact(15 * n, p)
                - _prime_exp_in_fact(10 * n, p)
                - _prime_exp_in_fact(7  * n, p)
                - _prime_exp_in_fact(6  * n, p);

        if (e > 0)
        {
            seq_t part = seq_pow((seq_t)p, e);
            result = seq_mul(result, part);
        }
    }

    /* Clean up the prime list. */
    free(primes);

    return result;
}
