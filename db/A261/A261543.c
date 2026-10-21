/*
 * A261543  Numbers of the form (prime(k) + Fibonacci(k))/2.
 * https://oeis.org/A261543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(A001651(n + 1)) + Fibonacci(A001651(n + 1)))/2
 *            A001651(m) is the m‑th positive integer >1 that is not divisible
 *            by 3, which can be computed as m + floor(m/2) + 1. Thus for
 *            a(n), k = n + floor(n/2) + 1. The function returns (prime(k) +
 *            Fib(k)) / 2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* k = n + floor(n/2) + 1 is the (n+1)-st element of A001651 */
    idx_t k = n + (n / 2) + 1;

    /* prime(k) as a 32‑bit value; cast to seq_t for addition */
    seq_t prime_val = (seq_t)get_prime(k);

    /* Fibonacci(k) as seq_t (may be big or small depending on build) */
    seq_t fib_val = fib(k);

    /* Return the arithmetic mean */
    return (prime_val + fib_val) / (seq_t)2U;
}
