/*
 * A377514  a(n) = number of iterations of x -> 2 x - 7 to reach a nonprime,
 *          starting with prime(n+4).
 * https://oeis.org/A377514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 5065, u32 n <= 6511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A377514: iterate x → 2x−7 until a non‑prime is reached,
   starting from the (n+4)‑th prime.  Return the number of iterations. */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Starting value: the (n+4)-th prime. */
    seq_t x = (seq_t)get_prime(n + 4);
    seq_t cnt = 0;

    /* Iterate while the current value is prime. */
    while (is_prime_big(x))
    {
        x = x * (seq_t)2 - (seq_t)7;
        cnt++;
    }

    return cnt;
}
