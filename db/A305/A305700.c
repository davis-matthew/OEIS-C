/*
 * A305700  a(n) is the numerator of Sum_{primes p < n} 1/(n-p).
 * https://oeis.org/A305700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{primes p < n} 1/(n-p)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 0, a(3) = 1, ... (as given in OEIS)      */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Handle the cases where there are no primes below n */
    if (n <= 2) return (seq_t)0;

    size_t count;
    uint64_t *primes = NULL;
    /* Build all primes strictly less than n */
    build_primes_upto((idx_t)n - 1, &primes, &count);

    seq_t num = 0;   /* current numerator of the partial sum */
    seq_t den = 1;   /* current denominator of the partial sum */

    for (size_t i = 0; i < count; ++i) {
        seq_t d = (seq_t)(n - primes[i]);   /* denominator of the new term */
        seq_t new_num = num * d + den;
        seq_t new_den = den * d;
        seq_t g = gcd(new_num, new_den);
        if (g != 0) {          /* avoid division by zero */
            new_num /= g;
            new_den /= g;
        }
        num = new_num;
        den = new_den;
    }

    free(primes);  /* deallocate prime array */
    return num;
}
