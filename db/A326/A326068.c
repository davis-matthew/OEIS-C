/*
 * A326068  a(n) = n - sigma(A032742(n)), where sigma is the sum of divisors
 *          of n and A032742 gives the largest proper divisor of n.
 * https://oeis.org/A326068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - sigma(largest proper divisor of n) * where sigma is
 *            the sum of all positive divisors. * Largest proper divisor of n
 *            is n / smallest prime factor of n, * except for n = 1, where it
 *            is defined to be 0
 * Range:     i8 n <= 130, i16 n <= 9911, i32 n <= 9911, i64 n <= 9911,
 *            i128 n <= 9911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            if ((mid + 1) > 0 && ((__int128)(mid + 1) * (mid + 1)) > n)
                return mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return 0; /* unreachable */
}

/* Smallest prime factor of n (for n >= 2). Returns n if n is prime. */
static uint64_t smallest_prime_factor(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= int_sqrt_u64(n); i += 2) {
        if (n % i == 0) return i;
    }
    return n;
}

/* Sum of divisors sigma of n (n >= 0). Handles n = 0 by returning 0. */
static seq_t sigma_u64(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 1;
    idx_t rem = n;

    for (idx_t p = 2; p <= int_sqrt_u64(rem); ++p) {
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            /* term = (p^(exp+1) - 1) / (p - 1) */
            seq_t power = 1;
            seq_t base = p;
            for (int e = 0; e < exp + 1; ++e)
                power *= base;
            seq_t term = (power - 1) / (p - 1);
            result *= term;
        }
    }
    if (rem > 1) { /* prime factor > sqrt */
        result *= (seq_t)rem + 1;
    }
    return result;
}

seq_t a(idx_t n)  /* external linkage as required */
{
    if (n == 1) return 0;  /* by definition in the OEIS entry */

    /* largest proper divisor: n / smallest prime factor */
    uint64_t sp = smallest_prime_factor(n);
    idx_t lpd = n / sp;

    seq_t sigma_lpd = sigma_u64(lpd);

    return (seq_t)n - sigma_lpd;
}
