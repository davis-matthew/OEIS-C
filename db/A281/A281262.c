/*
 * A281262  Number of permutations of [2n] with exactly n fixed points.
 * https://oeis.org/A281262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-2)*((n-1)*a(n-1)+(4*n-6)*a(n-2))/n (for n>1),
 *            a(0)=1, a(1)=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)0; /* a(1) */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t term1 = (seq_t)(4 * k - 2);
        seq_t part1 = (seq_t)(k - 1) * a_nm1;
        seq_t part2 = (seq_t)(4 * k - 6) * a_nm2;
        seq_t inside = part1 + part2;
        seq_t a_k = (term1 * inside) / (seq_t)k;
        a_nm2 = a_nm1;
        a_nm1 = a_k;
    }

    return a_nm1;
}
