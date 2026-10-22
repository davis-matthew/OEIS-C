/*
 * A377512  a(n) = number of iterations of x -> 2 x - 3 to reach a nonprime,
 *          starting with prime(n+2).
 * https://oeis.org/A377512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> 2x-3 to reach a nonprime,
 *            starting with prime(n+2)
 * Range:     u8 n <= 3, u16 n <= 3935, u32 n <= 3868, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Computes A377512: count iterations until a nonprime is obtained.
   The first term corresponds to n = 1 (prime(3) = 5). */
seq_t a(idx_t n)
{
    /* start with the (n+2)-th prime: n is 1‑based with offset 1 */
    idx_t prime_index = n + 2;            /* safe, wraps only for gigantic n */
    uint32_t p = get_prime(prime_index);  /* prime is < 2^32 for requested n */
    seq_t x = (seq_t)p;
    seq_t cnt = 0;

    while (is_prime_big(x))
    {
        x = x * 2 - 3;
        cnt++;
    }
    return cnt;
}
