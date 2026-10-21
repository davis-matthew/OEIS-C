/*
 * A277577  a(n) is the number of primes of the form (6n+3) +/- 3^k +/- 1,
 *          1<=k<(t-2) and t is the number of digits of (6n+3)'s balanced
 *          ternary representation.
 * https://oeis.org/A277577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A277577
    a(n) = number of primes among 6n+3 ± 3^k ± 1 for 1 ≤ k < t-2,
    where t is the number of balanced‑ternary digits of 6n+3.
*/

#include <stdbool.h>
#include <stdint.h>

/* number of digits of x in balanced ternary */
static idx_t balanced_digits(idx_t x)
{
    idx_t cnt = 0;
    while (x > 0) {
        idx_t r = x % 3;
        x /= 3;
        if (r == 2) {          /* -1 digit, carry 1 to next place */
            ++x;
        }
        ++cnt;
    }
    return cnt;
}

/* simple deterministic primality test for 64‑bit integers.
   It is more than fast enough for the numbers that occur in this sequence. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    if (n % 3 == 0)           return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* 6n + 3 fits comfortably in seq_t for the intended input ranges. */
    seq_t s = (seq_t)n * 6 + 3;
    idx_t  t = balanced_digits((idx_t)s);
    if (t < 3) return (seq_t)0;      /* no valid k */

    seq_t total = 0;
    uint64_t pow3 = 3;               /* 3^1 */

    for (idx_t k = 1; k <= t - 2; ++k, pow3 *= 3) {
        uint64_t c1 = (uint64_t)s + pow3 + 1;
        if (c1 > 1 && is_prime_uint64(c1)) ++total;

        uint64_t c2 = (uint64_t)s + pow3 - 1;
        if (c2 > 1 && is_prime_uint64(c2)) ++total;

        if ((uint64_t)s >= pow3) {
            uint64_t c3 = (uint64_t)s - pow3 + 1;
            if (c3 > 1 && is_prime_uint64(c3)) ++total;

            uint64_t c4 = (uint64_t)s - pow3 - 1;
            if (c4 > 1 && is_prime_uint64(c4)) ++total;
        }
    }

    return total;
}
