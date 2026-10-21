/*
 * A252854  Number of (n+2) X (1+2) 0..2 arrays with every consecutive three
 *          elements in every row, column, diagonal and antidiagonal having
 *          exactly two distinct values, and new values 0 upwards introduced
 *          in row major order.
 * https://oeis.org/A252854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3) + a(n-4) Initial values:
 *            a(1)=117, a(2)=279, a(3)=684, a(4)=1719 Computes the sequence
 *            for all n ≥ 1, using the recurrence which holds for n ≥ 5. The
 *            computation is O(n) and works for any seq_t that supports +, -,
 *            *, / with unsigned semantics
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n: return known values directly */
    if (n == 1) return (seq_t)117;
    if (n == 2) return (seq_t)279;
    if (n == 3) return (seq_t)684;
    if (n == 4) return (seq_t)1719;

    /* Use 4-term recurrence for n ≥ 5 */
    seq_t v0 = (seq_t)117;   /* a(n-4) */
    seq_t v1 = (seq_t)279;   /* a(n-3) */
    seq_t v2 = (seq_t)684;   /* a(n-2) */
    seq_t v3 = (seq_t)1719;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)4 * v3 - (seq_t)3 * v2 - (seq_t)2 * v1 + v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3;
}
