/*
 * A272231  a(n) = round(n / pi(n)) = round(n / A000720(n)) where round(x)
 *          rounds up.
 * https://oeis.org/A272231
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n / pi(n))
 * Range:     u8 n <= 1278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes the n-th term of OEIS sequence A272231.
 * a(n) = round(n / pi(n)),
 * where pi(n) is the number of primes ≤ n.
 * Implements rounding to the nearest integer (ties rounded up). */
seq_t a(idx_t n)
{
    if (n < 2) return 0;        /* undefined for n < 2; return 0 */

    size_t N = (size_t)n + 1;
    bool *is_prime = (bool *)malloc(N);
    if (!is_prime) return 0;    /* allocation failed */

    /* initialisation */
    for (size_t i = 0; i < N; ++i) is_prime[i] = true;
    if (N > 0) is_prime[0] = false;
    if (N > 1) is_prime[1] = false;

    /* simple sieve of Eratosthenes */
    for (size_t p = 2; p * p <= (size_t)n; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= (size_t)n; m += p) {
                is_prime[m] = false;
            }
        }
    }

    /* count primes up to n */
    seq_t pi = 0;
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (is_prime[i]) ++pi;
    }

    free(is_prime);

    /* round(n / pi) == (n + pi/2) / pi for positive integers */
    seq_t result = ((seq_t)n + (pi / 2)) / pi;
    return result;
}
