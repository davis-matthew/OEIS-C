/*
 * A296020  Number of primes of the form 4*k+3 <= 4*n+3.
 * https://oeis.org/A296020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p ≤ 4*n+3 with p ≡ 3 (mod 4)
 * Range:     u8 n <= 900, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count primes of the form 4*k+3 up to 4*n+3. */
seq_t a(idx_t n)
{
    /* Compute the upper bound for the sieve. */
    idx_t limit = 4 * n + 3;          /* 4*n+3 always fits in idx_t */

    /* Allocate a prime‑flag array.  Use a plain C array to avoid
       C++ type‑safety issues.  (void* → char*) cast is required in C++. */
    char *is_prime = (char*)malloc((size_t)(limit + 1));
    if (!is_prime) return (seq_t)0;    /* out‑of‑memory, return 0 */

    /* Initialise all entries to true. */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Simple Eratosthenes sieve. */
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t m = p * p; m <= limit; m += p)
            is_prime[m] = 0;
    }

    /* Count primes of the form 4*k+3.  These appear at indices
       3, 7, 11, … so we can skip by 4. */
    seq_t count = 0;
    for (idx_t i = 3; i <= limit; i += 4)
        if (is_prime[i]) ++count;

    free(is_prime);
    return count;
}
