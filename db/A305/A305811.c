/*
 * A305811  Filter sequence for a(Fibonacci prime) = constant sequences.
 * https://oeis.org/A305811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n)=2 if n is a Fibonacci prime; otherwise
 *            a(n)=n - (number of Fibonacci primes ≤ n) + 1
 * Range:     u8 n <= 259, u16 n <= 9391, u32 n <= 9391, u64 n <= 9391,
 *            u128 n <= 9391, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

/* Helper: test whether a given value is a Fibonacci prime. */
static bool is_fibonacci_prime(seq_t x)
{
    if (x < 2)
        return false;
    seq_t a = 0, b = 1;           /* Fibonacci numbers F(0)=0, F(1)=1 */
    while (b <= x) {
        if (b == x)
            return is_prime_big(b);   /* b is Fibonacci; check primality */
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 1)          /* offset 1 */
        return (seq_t)1;

    seq_t sn = (seq_t)n;

    /* Special value for Fibonacci primes */
    if (is_fibonacci_prime(sn))
        return (seq_t)2;

    /* Count Fibonacci primes ≤ n */
    seq_t count = 0;
    seq_t a_fib = 0, b_fib = 1;   /* same generation of Fibonacci numbers */
    while (b_fib <= sn) {
        if (b_fib >= 2 && is_prime_big(b_fib))
            ++count;
        seq_t tmp = a_fib + b_fib;
        a_fib = b_fib;
        b_fib = tmp;
    }

    /* Running count for non‑Fibonacci‑prime indices, starting at 3 */
    return sn - count + 1;
}
