/*
 * A332032  G.f.: Sum_{k>=1} k! * x^(k^2) / (1 - x^(2*k)).
 * https://oeis.org/A332032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d <= n/d, d and n/d have same parity as n} d!
 * Range:     u8 n <= 35, u16 n <= 80, u32 n <= 168, u64 n <= 440,
 *            u128 n <= 1224, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A simple O(sqrt(n)) computation that works for all n >= 1.
   It iterates over all divisors d up to floor(sqrt(n)) and adds
   d! to the result when the parity condition is satisfied. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Guard for zero or negative arguments. */

    /* Compute floor(sqrt(n)) using a small integer loop (n <= 10^4 usually). */
    idx_t sqrt_n = 0;
    while (((sqrt_n + 1) * (sqrt_n + 1)) <= n) ++sqrt_n;

    seq_t sum = 0;

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d != 0) continue;                 /* d is not a divisor. */
        idx_t e = n / d;                           /* Complementary divisor. */

        /* Check parity condition: d % 2 == n % 2  &&  e % 2 == n % 2. */
        if (((d & 1) == (n & 1)) && ((e & 1) == (n & 1))) {
            /* Compute d! iteratively.  d is at most sqrt_n (<= 100 for n <= 10^4). */
            seq_t fact = 1;
            for (idx_t i = 2; i <= d; ++i) fact *= (seq_t)i;
            sum += fact;
        }
    }

    return sum;
}
