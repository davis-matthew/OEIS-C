/*
 * A308567  Consider the second least-significant bits of the first n prime
 *          numbers: a(n) equals the number of zeros minus the number of ones.
 * https://oeis.org/A308567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = (# of zeros – # of ones) in the second least–significant
 *            bits of the first n primes
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* only defined for n >= 1; safety */

    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = primes[i];
        /* bit 1 (second LSB): 0 → zero, 1 → one */
        if ((p >> 1) & 1u)
            sum -= 1;   /* one contributes -1 */
        else
            sum += 1;   /* zero contributes +1 */
    }

    free(primes);
    return sum;
}
