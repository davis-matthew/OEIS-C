/*
 * A290584  a(n) is the factor P(n) having prime factors between n^2 and 2*n^2
 *          in A285388(n) = R(n)P(n) for n > 1, a(1)=1.
 * https://oeis.org/A290584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
**  Sequence A290584:  a(n) = product of all primes p with n² ≤ p ≤ 2·n²  
**  
**  Formula:  a(1)=1; for n>1  
**      a(n) = ∏ { p prime | n*n ≤ p ≤ 2*n*n }  
**  
**  The implementation builds a global list of primes up to the largest
**  upper bound requested, and then multiplies only those primes falling
**  inside the required interval.  A large prime sieve is required only
**  once per execution, so subsequent calls are very fast.
**  
**  All arithmetic that can overflow the native integer type is done in
**  *seq_t* (which may be an arbitrary‑precision class in the C++ build).
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 1 */
    if (n <= 1)
        return (seq_t)1;

    /* compute the interval [n², 2·n²] as idx_t values */
    idx_t sq = (idx_t)n * (idx_t)n;
    idx_t low = sq;
    idx_t high = sq * 2;

    /* keep a static prime table that is enlarged only when necessary */
    static idx_t primes_limit = 0;
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    if (high > primes_limit)
    {
        /* free previous table if it exists */
        if (prime_list)
            free(prime_list);
        /* generate new prime list up to the new limit */
        build_primes_upto(high, &prime_list, &prime_cnt);
        primes_limit = high;
    }

    /* multiply primes that lie inside the interval */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if ((seq_t)p < low)
            continue;
        if ((seq_t)p > high)
            break;
        result *= (seq_t)p;
    }
    return result;
}
