/*
 * A268731  Period of the decimal expansion of 1/h(n) where h(n) is the length
 *          of the finite sequence {n, f(n), f(f(n)),...,1} in the Collatz (or
 *          3n + 1) problem.
 * https://oeis.org/A268731
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = period of the decimal expansion of 1 / h(n) where h(n) is
 *            the total stopping time of n in the Collatz (3n+1) problem, i.e.
 *            the number of steps needed to reach 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Collatz total stopping time of n (n ≥ 2) and then
   returns the period of 1 / h(n) in base‑10.  The period is defined
   to be 1 when the decimal expansion terminates. */
seq_t a(idx_t n)
{
    /* Compute the Collatz stopping time (number of steps to reach 1). */
    uint64_t x = (uint64_t)n;
    uint64_t steps = 0;
    while (x != 1) {
        if ((x & 1ULL) == 0)
            x >>= 1;               /* n even: n / 2 */
        else
            x = 3 * x + 1;          /* n odd: 3n + 1 */
        ++steps;
    }

    /* Compute the decimal period of 1 / steps. */
    uint64_t m = steps;
    if (m == 1)
        return (seq_t)1;            /* 1/1 has “period” 1 by definition */

    /* Strip factors of 2 and 5; a terminating fraction has period 1. */
    while ((m & 1ULL) == 0)
        m >>= 1;
    while ((m % 5ULL) == 0)
        m /= 5ULL;

    if (m == 1)
        return (seq_t)1;            /* terminating decimal */

    /* Find the multiplicative order of 10 modulo m. */
    uint64_t r = 10 % m;
    uint64_t period = 1;
    while (r != 1) {
        r = (r * 10ULL) % m;
        ++period;
    }
    return (seq_t)period;
}
