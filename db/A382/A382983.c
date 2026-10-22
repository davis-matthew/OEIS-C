/*
 * A382983  a(n) is the number of solutions to n = x*y in positive integers x
 *          <= y where x + y is prime.
 * https://oeis.org/A382983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of factorizations n = x*y with x ≤ y such that x +
 *            y is prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count pairs (x, y) with x*y = n, x ≤ y and x + y prime. */
#include <stdbool.h>

/* Lightweight primality test for idx_t values. */
static bool is_prime_local(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* count fits easily into seq_t */
    seq_t count = 0;
    for (idx_t x = 1; x <= n / x; ++x) {
        if (n % x == 0) {
            idx_t y = n / x;               /* x * y = n and y ≥ x   */
            if (is_prime_local(x + y)) {
                ++count;
            }
        }
    }
    return count;
}
