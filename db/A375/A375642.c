/*
 * A375642  a(n) is the number of i for which n - Fibonacci(i) is prime.
 * https://oeis.org/A375642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of i ≥ 0 such that n − Fibonacci(i) is prime
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Small prime test for numbers up to about 1e9 (works for all inputs of this sequence). */
static bool is_prime_small(uint64_t v)
{
    if (v < 2) return false;
    if (v == 2) return true;
    if ((v & 1) == 0) return false;
    for (uint64_t d = 3; d <= v / d; d += 2)
        if (v % d == 0) return false;
    return true;
}

/* Return the sequence value a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Handle the trivial case n < 2 quickly. */
    if (n < 2) return (seq_t)0;

    /* Maximum Fibonacci index needed is small (≤ 20 for n ≤ 10000). */
    const idx_t MAX_FIB = 30;
    seq_t fib[MAX_FIB];
    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;
    idx_t fib_count = 2;
    seq_t limit = (seq_t)(n - 2);

    /* Build Fibonacci numbers up to the limit. */
    while (fib_count < MAX_FIB) {
        seq_t next = fib[fib_count - 1] + fib[fib_count - 2];
        if (next > limit) break;
        fib[fib_count++] = next;
    }

    /* Count indices i with n − fib[i] prime. */
    seq_t count = 0;
    for (idx_t i = 0; i < fib_count; ++i) {
        seq_t diff = (seq_t)n - fib[i];
        if (is_prime_small((uint64_t)diff)) count++;
    }

    return count;
}
