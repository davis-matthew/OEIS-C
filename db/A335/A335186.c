/*
 * A335186  a(n) = nextprime(ceiling(n/2)-1) + prevprime(floor(n/2)+1), where
 *          nextprime = A151800 and prevprime = A151799.
 * https://oeis.org/A335186
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nextprime(ceil(n/2)-1) + prevprime(floor(n/2)+1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 74 (2026-11-01)
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 4. For lower indices return 0
       to avoid unsigned underflow in the component calculations. */
    if (n < 4)
        return (seq_t)0;

    /* Compute floor(n/2) and ceil(n/2) using bit operations. */
    idx_t floor_half = n >> 1;          /* floor(n/2) */
    idx_t ceil_half  = (n + 1) >> 1;    /* ceil(n/2)  */

    /* Arguments for the prime lookups. */
    idx_t arg_next = ceil_half - 1;     /* ceil(n/2) - 1 */
    idx_t arg_prev = floor_half + 1;    /* floor(n/2) + 1 */

    /* Retrieve the primes.  The utilities return 64‑bit values,
       which we cast to seq_t for the final addition. */
    seq_t p_next = (seq_t)next_prime(arg_next);
    seq_t p_prev = (seq_t)prev_prime(arg_prev);

    /* Sum the primes to obtain a(n). */
    return p_next + p_prev;
}
