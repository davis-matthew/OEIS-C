/*
 * A348110  Number of positive integers <= n that have middle divisors.
 * https://oeis.org/A348110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k <= n that have a middle divisor (i.e., there
 *            exists a divisor d of k with sqrt(k/2) <= d <= sqrt(k))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Check if a given number n has a middle divisor */
static bool has_middle_divisor(idx_t n)
{
    if (n == 0)
        return false;

    /* Compute floor(sqrt(n)) */
    long double sqrt_n_ld = sqrtl((long double)n);
    idx_t sqrt_n = (idx_t)sqrt_n_ld;

    /* Compute ceil(sqrt(n/2)) */
    long double half_ld = sqrtl((long double)n / 2.0L);
    idx_t start = (idx_t)ceill(half_ld);

    if (start > sqrt_n)
        return false;

    for (idx_t d = start; d <= sqrt_n; ++d) {
        if (n % d == 0)
            return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (has_middle_divisor(i))
            ++count;
    }
    return count;
}
