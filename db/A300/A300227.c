/*
 * A300227  a(n) = number of steps in simple Euclidean algorithm for gcd(n,k)
 *          to reach the termination test n=k when starting with n = n and k =
 *          sigma(n)-1, with a(1) = 1.
 * https://oeis.org/A300227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps in the subtractive Euclidean algorithm
 *            for gcd(n, sigma(n)-1) until the two numbers become equal
 *            (termination test n == k). a(1) = 1
 * Range:     u8 n <= 463, u16 n <= 8835, u32 n <= 8835, u64 n <= 8835,
 *            u128 n <= 8835, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes the sum of divisors of n, subtracts
   one, then repeatedly subtracts the smaller value from the larger
   until equality, counting the operations. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute σ(n) (sum of positive divisors of n) using a
   64‑bit integer.  The result fits into uint64_t for the n that
   will be used in the tests (≤ 10 000). */
static uint64_t sigma_uint64(idx_t n)
{
    unsigned __int128 sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            sum += (unsigned __int128)i;
            idx_t j = n / i;
            if (j != i) sum += (unsigned __int128)j;
        }
    }
    return (uint64_t)sum;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* a(n) is the number of subtraction steps until (a, b) are equal,
       where a = n and b = σ(n) - 1. */
    uint64_t a_val = (uint64_t)n;
    uint64_t b_val = sigma_uint64(n) - (uint64_t)1;

    uint64_t steps = 0;
    while (a_val != b_val)
    {
        if (a_val > b_val)
            a_val -= b_val;
        else
            b_val -= a_val;
        ++steps;
    }

    return (seq_t)steps;
}
