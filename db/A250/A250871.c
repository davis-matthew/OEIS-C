/*
 * A250871  Number of (n+1) X (2+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (472*4^n - 378*3^n + 54*2^n + 2)/3 Recurrence (valid for
 *            n ≥ 5): a(n) = 10*a(n‑1) – 35*a(n‑2) + 50*a(n‑3) – 24*a(n‑4)
 *            Initial values: a(1) = 288 a(2) = 1456 a(3) = 6812 a(4) = 30360
 *            The function computes a(n) for any n ≥ 1 using the recurrence
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)288;
    if (n == 2u) return (seq_t)1456;
    if (n == 3u) return (seq_t)6812;
    if (n == 4u) return (seq_t)30360;

    seq_t a1 = (seq_t)288, a2 = (seq_t)1456,
          a3 = (seq_t)6812, a4 = (seq_t)30360;

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = (seq_t)(10 * a4 - 35 * a3 + 50 * a2 - 24 * a1);
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
