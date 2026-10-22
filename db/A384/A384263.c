/*
 * A384263  a(n) = Product_{k=0..n-1} (3*n+k-1).
 * https://oeis.org/A384263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} (3*n + k - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   _factorial_exponent, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/_factorial_exponent.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* offset 0 */

    /* a(n) = (4n-2)!/(3n-2)! */
    idx_t upper = 4 * n - 2;
    idx_t lower = 3 * n - 2;
    size_t lim = (size_t)upper;

    /* ---------- sieve up to upper ---------- */
    bool *is_prime = (bool *)malloc((lim + 1) * sizeof(bool));
    if (!is_prime) return (seq_t)0;          /* OOM guard */
    for (size_t i = 0; i <= lim; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t p = 2; (size_t)p * (size_t)p <= lim; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= lim; j += p) is_prime[j] = false;
        }
    }

    /* ---------- compute product via prime exponents ---------- */
    seq_t result = 1;
    for (idx_t p = 2; p <= lim; ++p) {
        if (is_prime[p]) {
            idx_t exp = _factorial_exponent(upper, p) - _factorial_exponent(lower, p);
            if (exp) {
                seq_t powp = big_pow((seq_t)p, (seq_t)exp);
                result *= powp;
            }
        }
    }

    free(is_prime);
    return result;
}
