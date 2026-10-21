/*
 * A285809  Primes equal to a centered triangular number plus 1.
 * https://oeis.org/A285809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A285809: primes equal to 3*n*(n-1)/2 + 2 */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static state to memoize results among calls */
    static seq_t *primes = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_n_test = 1; /* next n to try in the quadratic formula */

    while (len < n) {
        seq_t temp = (seq_t)next_n_test * ((seq_t)next_n_test - (seq_t)1);
        temp /= (seq_t)2;                     /* n*(n-1)/2   */
        seq_t value = temp * (seq_t)3 + (seq_t)2; /* 3*n*(n-1)/2 + 2 */

        if (is_prime_big(value))
            append_candidate(&primes, &len, &cap, value);

        next_n_test++;
    }
    return primes[n - 1];
}
