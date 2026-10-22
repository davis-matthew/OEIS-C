/*
 * A399155  a(n) = f(n) - g(n), where f(n) is the number of steps to reach 0
 *          by repeatedly subtracting the smallest prime factor of the current
 *          value, and g(n) is the number of steps to reach 0 by repeatedly
 *          subtracting the largest prime factor.
 * https://oeis.org/A399155
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = steps using the smallest prime factor – steps using the
 *            largest prime factor
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest prime factor by trial division.  For n ≥ 2 this
   function always returns a prime divisor of n. */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return d;
    return n;   /* n is prime */
}

/* Compute the largest prime factor by trial division.  The function
   returns the greatest prime divisor of n (n ≥ 2). */
static uint64_t largest_prime_factor(uint64_t n)
{
    uint64_t max_pf = 1ULL;
    /* factor out 2s first */
    while (n % 2ULL == 0ULL) {
        max_pf = 2ULL;
        n /= 2ULL;
    }
    /* factor odd divisors */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        while (n % d == 0ULL) {
            max_pf = d;
            n /= d;
        }
    }
    if (n > 1ULL) max_pf = n;
    return max_pf;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* Offset is 2 – values below are irrelevant */

    uint64_t val = (uint64_t)n;
    seq_t steps_min = 0;
    seq_t steps_max = 0;

    /* Steps using the smallest prime factor */
    uint64_t tmp = val;
    while (tmp > 0) {
        uint64_t spf = smallest_prime_factor(tmp);
        tmp -= spf;
        ++steps_min;
    }

    /* Steps using the largest prime factor */
    tmp = val;
    while (tmp > 0) {
        uint64_t lpf = largest_prime_factor(tmp);
        tmp -= lpf;
        ++steps_max;
    }

    /* The conjecture states the result is always non‑negative. */
    return steps_min - steps_max;
}
