/*
 * A246348  a(1)=1, a(p_n) = 1 + a(n), a(c_n) = 1 + a(n), where p_n = n-th
 *          prime = A000040(n), c_n = n-th composite number = A002808(n); Also
 *          binary width of terms of A135141.
 * https://oeis.org/A246348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(index(n)) where * index(n) = number of primes ≤ n
 *            if n is prime * index(n) = n − 1 − number of primes ≤ n if n is
 *            composite * a(1) = 1
 * Range:     u8 n <= 8626, u16 n <= 8626, u32 n <= 8626, u64 n <= 8626,
 *            u128 n <= 8626, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  // Not defined for n<1, but guard

    /* Allocate arrays up to n (inclusive) */
    size_t N = (size_t)n;
    bool *is_prime = (bool*)malloc((N + 1) * sizeof(bool));
    if (!is_prime) return 0;   /* allocation failure */

    /* Sieve of Eratosthenes */
    for (size_t i = 0; i <= N; ++i) is_prime[i] = true;
    if (N >= 0) is_prime[0] = false;
    if (N >= 1) is_prime[1] = false;
    for (size_t p = 2; p * p <= N; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= N; m += p) is_prime[m] = false;
        }
    }

    /* Prefix counts of primes */
    idx_t *pi = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    if (!pi) { free(is_prime); return 0; }
    pi[0] = 0;
    for (size_t i = 1; i <= N; ++i) {
        pi[i] = pi[i - 1] + (is_prime[i] ? 1 : 0);
    }

    /* Compute a(n) for all 1..n */
    seq_t *a_arr = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!a_arr) { free(is_prime); free(pi); return 0; }
    a_arr[0] = 0;          /* unused */
    a_arr[1] = 1;          /* base case */

    for (size_t i = 2; i <= N; ++i) {
        if (is_prime[i]) {
            a_arr[i] = (seq_t)1 + a_arr[(size_t)pi[i]];
        } else {
            idx_t compIdx = (idx_t)i - 1 - pi[i];
            a_arr[i] = (seq_t)1 + a_arr[(size_t)compIdx];
        }
    }

    seq_t result = a_arr[(size_t)n];

    free(is_prime);
    free(pi);
    free(a_arr);
    return result;
}
