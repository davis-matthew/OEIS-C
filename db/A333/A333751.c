/*
 * A333751  Sum of nonprime divisors of n that are <= sqrt(n).
 * https://oeis.org/A333751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sum of nonprime divisors of n that are <= sqrt(n)
 * Range:     u8 n <= 1679, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t floor_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= n / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

static inline bool is_prime_small(idx_t v)
{
    if (v < 2) return false;
    if (v == 2 || v == 3) return true;
    if ((v & 1ULL) == 0 || v % 3 == 0) return false;
    idx_t lim = floor_sqrt(v);
    for (idx_t i = 5; i <= lim; i += 6) {
        if (v % i == 0 || v % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Sum all divisors d of n satisfying d <= sqrt(n) and !is_prime(d) */
    idx_t limit = floor_sqrt(n);
    seq_t sum = 0;
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            if (!is_prime_small(d)) sum += (seq_t)d;
        }
    }
    return sum;
}
