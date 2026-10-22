/*
 * A351366  a(n) = Sum_{p|n, p prime} p^p.
 * https://oeis.org/A351366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^p where the sum runs over distinct
 *            prime divisors of n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, floor_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_isqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

/* Static storage for the prime list used to factor numbers. */
static idx_t      _prime_limit = 0;       /* maximum number for which primes are known */
static uint64_t  *_prime_list  = NULL;    /* array of primes */
static size_t    _prime_count = 0;        /* number of primes in the array */

/*-----------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2)           /* by definition a(1) = 0 */
        return 0;

    /* Ensure we have all primes up to ⌊√n⌋. */
    idx_t limit = floor_isqrt(n);          /* correct function from utils */
    if (limit > _prime_limit) {
        free(_prime_list);                 /* free old list if any */
        build_primes_upto(limit, &_prime_list, &_prime_count);
        _prime_limit = limit;
    }

    /* Factor n and sum p^p for each distinct prime factor p. */
    idx_t temp = n;
    seq_t sum  = 0;

    for (size_t i = 0; i < _prime_count; ++i) {
        idx_t p = _prime_list[i];
        if (p * p > temp)                /* no further small factors */
            break;
        if (temp % p == 0) {
            sum += big_pow((seq_t)p, (seq_t)p);
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* If a prime factor larger than √n remains, it appears once. */
    if (temp > 1)
        sum += big_pow((seq_t)temp, (seq_t)temp);

    return sum;
}
