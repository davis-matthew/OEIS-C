/*
 * A337771  Number of positive integer pairs, (s,t), with s,t composite, such
 *          that s < t < n, and neither s nor t divides n.
 * https://oeis.org/A337771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C({ i composite < n : n % i != 0 }, 2)
 * Range:     u8 n <= 36, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* No pairs can exist for n <= 4 */
    if (n <= 4)
        return (seq_t)0;

    /* Allocate an array to mark primality up to n-1 */
    size_t limit = (size_t)n;
    bool *is_prime = (bool *)malloc(limit * sizeof(bool));
    if (!is_prime)
        return (seq_t)0; /* allocation failure – safe fallback */

    /* Initialize all entries as prime first, then correct */
    for (size_t i = 0; i < limit; ++i)
        is_prime[i] = true;
    if (limit > 0) is_prime[0] = false;
    if (limit > 1) is_prime[1] = false;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i < (idx_t)limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j < (idx_t)limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    /* Count composite numbers < n that do not divide n */
    idx_t cnt = 0;
    for (idx_t i = 4; i < n; ++i) {
        if (!is_prime[(size_t)i] && (n % i) != 0)
            ++cnt;
    }

    free(is_prime);

    if (cnt < 2)
        return (seq_t)0;

    seq_t c = (seq_t)cnt;
    return (c * (c - (seq_t)1)) / (seq_t)2;
}
