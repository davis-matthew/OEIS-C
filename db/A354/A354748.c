/*
 * A354748  a(n) is the prime reached after A354747(n) steps when repeatedly
 *          applying the map x -> 3*x+2 to 2*n-1, or 0 if no prime is ever
 *          reached.
 * https://oeis.org/A354748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 20, u32 n <= 20, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the prime reached after repeatedly applying x → 3*x+2 to 2*n−1 until a prime is reached.
   Returns 0 if no prime is ever reached (practical for very large n). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Start with the first iterate of 2*n-1 under the map f(x)=3*x+2. */
    seq_t x = (seq_t)3 * ((seq_t)n * 2 - 1) + 2;
    /* To guard against an infinite loop for exceptional inputs we limit the
       number of iterations.  The limit is chosen far above any known step
       count for n ≤ 10000; it can be raised if needed. */
    const unsigned long long MAX_STEPS = 1000000ULL;
    unsigned long long steps = 0;

    while (1) {
        if (is_prime_big(x))
            return x;
        if (steps >= MAX_STEPS)
            return (seq_t)0;          /* assume no prime found within reasonable steps */
        x = (seq_t)3 * x + 2;
        ++steps;
    }
}
