/*
 * A333810  G.f.: Sum_{k>=1} (-1)^(k + 1) * k * x^(k*(k + 1)) / (1 - x^k).
 * https://oeis.org/A333810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n , d<√n} { +d if d is odd, –d if d is even }
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the excess of the sum of odd divisors of n that are
   strictly less than √n over the sum of even divisors less than √n.
   The function works for any n ≥ 1 and uses only integer arithmetic. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 we simply return 0. */
    if (n == 0)
        return 0;

    /* Limit = floor(√(n‑1)).  Only divisors strictly < √n are counted. */
    idx_t m = n - 1;
    idx_t limit = 0;
    while (((__int128)(limit + 1) * (limit + 1)) <= m) {
        ++limit;
    }

    /* Accumulate the weighted divisor sum. */
    seq_t sum = 0;
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            if (d & 1)            /* odd divisor  → add */
                sum += (seq_t)d;
            else                  /* even divisor → subtract */
                sum -= (seq_t)d;
        }
    }
    return sum;
}
