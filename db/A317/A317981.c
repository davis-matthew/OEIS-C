/*
 * A317981  Expansion of x*(125 + 8028*x + 42237*x^2 + 42272*x^3 + 8007*x^4 +
 *          132*x^5 - x^6) / (1 - x)^8.
 * https://oeis.org/A317981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*n^7 + 70*n^6 + 70*n^5 - 28*n^3 - 7*n^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 15, u64 n <= 367, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;
    seq_t N6 = N5 * N;
    seq_t N7 = N6 * N;

    seq_t t1 = 20 * N7;
    seq_t t2 = 70 * N6;
    seq_t t3 = 70 * N5;
    seq_t t4 = 28 * N3;
    seq_t t5 = 7  * N2;

    seq_t sum = t1 + t2 + t3;
    sum = sum - t4 - t5;
    return sum;
}
