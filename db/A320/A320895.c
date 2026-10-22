/*
 * A320895  a(n) = Sum_{k=1..n} k^3 * tau(k), where tau is A000005.
 * https://oeis.org/A320895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^3 * tau(k) Computed via divisor-sum trick:
 *            a(n) = Sum_{d=1..n} d^3 * ( floor(n/d)*(floor(n/d)+1)/2 )^2 The
 *            implementation groups ranges of d with equal floor(n/d) to run
 *            in O(sqrt n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 227, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ) {
        idx_t t = n / i;          /* floor(n / i) */
        idx_t j = n / t;          /* last d with this same t */

        /* sum of cubes from i to j:
           S(x) = (x*(x+1)/2)^2   =>  sum_{d=i}^j d^3 = S(j) - S(i-1) */
        seq_t sj = (seq_t)j * (j + 1) / 2;
        sj = sj * sj;             /* S(j) */
        seq_t si_minus = 0;
        if (i > 1) {
            seq_t tmp = (seq_t)(i - 1) * i / 2;
            si_minus = tmp * tmp; /* S(i-1) */
        }
        seq_t sum_cubes = sj - si_minus;

        /* inner value: (t*(t+1)/2)^2 */
        seq_t inner = (seq_t)t * (t + 1) / 2;
        inner = inner * inner;

        result += sum_cubes * inner;
        i = j + 1;
    }
    return result;
}
