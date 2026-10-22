/*
 * A308572  a(n) = Fibonacci(2*prime(n)).
 * https://oeis.org/A308572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2*prime(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Convert the n-th prime to a 64‑bit index for the Fibonacci routine */
    uint32_t prime = get_prime(n);
    idx_t idx = ((idx_t)prime) << 1;          /* idx = 2 * prime(n) */
    return fib(idx);
}
