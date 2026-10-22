/*
 * A356727  Primes of the form 4*k^2 + 84*k + 43.
 * https://oeis.org/A356727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   primes of the form 4*k^2 + 84*k + 43
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th prime produced by P(k) = 4*k^2+84*k+43   */
/* a(1) = 43, a(2) = 131, …                                   */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of primes already found                     */
    static seq_t *cache   = NULL;    /* array of primes      */
    static size_t count  = 0;        /* number of primes     */
    static size_t capacity = 0;      /* allocated capacity   */
    static idx_t  last_k = 0;        /* last k examined      */

    /* Undefined for n == 0 (offset starts at 1)          */
    if (n == 0) return 0;

    /* Extend cache until it contains at least n primes */
    if (count < (size_t)n)
    {
        while (count < (size_t)n)
        {
            /* Ensure enough storage for the next prime */
            if (count == capacity)
                grow_seq(&cache, &capacity, capacity > 0 ? capacity * 2 : 1);

            /* Compute polynomial value for current k   */
            seq_t k   = (seq_t)last_k;
            seq_t val = 4 * k * k + 84 * k + 43;

            /* Keep it if it is prime                   */
            if (is_prime_big(val))
                cache[count++] = val;

            ++last_k;
        }
    }

    return cache[n - 1];
}
