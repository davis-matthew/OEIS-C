/*
 * A331959  a(n) is the greatest prime number of the form floor(n/k) where k >
 *          0.
 * https://oeis.org/A331959
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime among floor(n/k) for k > 0 (k integer)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test for primality of values that fit in uint64_t.
   Works for the sizes needed in this sequence (n <= 10000). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i <= x / i; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2, behaviour below not critical */

    seq_t best = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t val = (seq_t)(n / k);          /* floor(n/k) */
        if (val > best && is_prime_uint64((uint64_t)val)) {
            best = val;
            if (best == n) break;            /* n itself is prime — cannot improve */
        }
    }
    return best;
}
