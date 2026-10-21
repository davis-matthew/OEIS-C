/*
 * A241424  Largest number k > 0 such that n - k! is prime, or 0 if no such k
 *          exists.
 * https://oeis.org/A241424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k > 0 such that n - k! is prime, else 0
 * Range:     u8 n <= 1986, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Compute a(n) = largest k>0 with n - k! prime, otherwise 0. */
seq_t a(idx_t n)
{
    /* For n < 3, no k yields n - k! ≥ 2. */
    if (n < 3)
        return (seq_t)0;

    /* The maximum factorial to test is the largest k! ≤ n-2. */
    seq_t limit = (seq_t)(n - 2);

    /* Precompute factorials up to the point where k! > limit. */
    /* 400 is enough for n up to about 10^1000; 400×sizeof(seq_t)
       is small (< 20 KB for common sizes). */
    seq_t factorials[400];
    idx_t k = 1;          /* 1! is always ≤ limit for n ≥ 3. */
    seq_t fact = 1;       /* factorial of current k */
    factorials[1] = fact;

    while (1)
    {
        idx_t next_k = k + 1;
        seq_t next_fact = fact * (seq_t)next_k;

        if (next_fact > limit)
            break;       /* next factorial exceeds limit; stop. */

        ++k;
        fact = next_fact;
        factorials[k] = fact;
    }

    /* Search downward for the largest k with n - k! prime. */
    for (idx_t idx = k; idx >= 1; --idx)
    {
        seq_t diff = (seq_t)n - factorials[idx];
        if (is_prime_big(diff))
            return (seq_t)idx;
    }

    return (seq_t)0;
}
