/*
 * A376085  a(0..5) = 1 and a(n) = 1 - a(n-1) - a(n-2) +
 *          a(n-1)*a(n-2)*a(n-3)/a(n-4) + a(n-2)*a(n-3)*a(n-4)/a(n-5) +
 *          a(n-3)*a(n-4)*a(n-5)/a(n-6), for n > 5.
 * https://oeis.org/A376085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0..5) = 1; a(n) = 1 - a(n-1) - a(n-2) +
 *            a(n-1)*a(n-2)*a(n-3)/a(n-4) + a(n-2)*a(n-3)*a(n-4)/a(n-5) +
 *            a(n-3)*a(n-4)*a(n-5)/a(n-6)
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 12, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A376085. */
seq_t a(idx_t n)
{
    if (n <= 5)
        return (seq_t)1;

    seq_t prev[7];
    for (idx_t i = 0; i < 6; ++i)
        prev[i] = (seq_t)1;

    for (idx_t k = 6; k <= n; ++k) {
        seq_t a1 = prev[(k - 1) % 7];
        seq_t a2 = prev[(k - 2) % 7];
        seq_t a3 = prev[(k - 3) % 7];
        seq_t a4 = prev[(k - 4) % 7];
        seq_t a5 = prev[(k - 5) % 7];
        seq_t a6 = prev[(k - 6) % 7];

        seq_t part1 = a1 * a2;
        part1 *= a3;
        part1 /= a4;

        seq_t part2 = a2 * a3;
        part2 *= a4;
        part2 /= a5;

        seq_t part3 = a3 * a4;
        part3 *= a5;
        part3 /= a6;

        seq_t res = part1 + part2 + part3 + (seq_t)1;
        res -= a1;
        res -= a2;

        prev[k % 7] = res;
    }

    return prev[n % 7];
}
