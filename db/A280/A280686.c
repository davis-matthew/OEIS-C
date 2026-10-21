/*
 * A280686  Largest Fibonacci proper divisor of n, a(1) = 1.
 * https://oeis.org/A280686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest Fibonacci proper divisor of n, i.e. the greatest
 *            Fibonacci number < n that divides n
 * Range:     u8 n <= 753, u16 n <= 8463, u32 n <= 8463, u64 n <= 8463,
 *            u128 n <= 8463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    /* cache Fibonacci numbers up to the maximum needed.
       At most 200 Fibonacci numbers fit in any 128‑bit integer. */
    static seq_t fib[200];
    static size_t fib_size = 0;

    if (fib_size == 0) {
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)1;
        fib_size = 2;
    }

    /* Extend the cache until the last stored Fibonacci number is
       not less than n. */
    while (fib_size < 200 && fib[fib_size - 1] < (seq_t)n) {
        seq_t next = fib[fib_size - 1] + fib[fib_size - 2];
        /* Stop if addition overflowed (next <= previous). */
        if (next <= fib[fib_size - 1])
            break;
        fib[fib_size++] = next;
    }

    /* Find the largest Fibonacci less than n that divides n. */
    for (size_t i = fib_size; i > 0; --i) {
        seq_t f = fib[i - 1];
        if (f < (seq_t)n && (n % f == 0))
            return f;
    }
    /* Should never reach here; 1 always divides n. */
    return (seq_t)1;
}
