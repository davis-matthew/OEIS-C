/*
 * A274849  Isolated deficient numbers.
 * https://oeis.org/A274849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 5292, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274849 – Isolated deficient numbers
 *
 * Computation: iterate over integers k ≥ 2, test
 *   – k is deficient  (σ(k) < 2k)
 *   – k-1 is NOT deficient (σ(k-1) ≥ 2(k-1))
 *   – k+1 is NOT deficient (σ(k+1) ≥ 2(k+1))
 *
 * The nth isolated deficient number is returned for each call.
 * The routine recomputes from scratch on each call; this is fast
 * enough for n ≤ 10000 and requires no external libraries.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* sum of all positive divisors of n (including n itself) */
static uint64_t sigma_uint64(uint64_t n)
{
    uint64_t sum = 0;
    uint64_t i;
    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            sum += i;
            uint64_t j = n / i;
            if (j != i)
                sum += j;
        }
    }
    return sum;
}

/* true iff n is deficient (sum of proper divisors < n) */
static inline bool deficient(uint64_t n)
{
    /* σ(n) = sum of all divisors; σ(n) < 2n  ⇔  sum of proper divisors < n */
    return sigma_uint64(n) < (2 * n);
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset is 1 – never used for n=0 */

    size_t found = 0;
    for (uint64_t k = 2; ; ++k)
    {
        if (deficient(k) && !deficient(k - 1) && !deficient(k + 1))
        {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
}
