/*
 * A250762  Number of (7+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 134, u64 n <= 134, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250762 (Number of (7+1) X (n+1) 0..2 arrays)
 *
 * Closed form:  a(n) = (15893/2)*n^2 + (29009/2)*n + 6561
 * Recurrence (valid for n > 3):  a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3)
 * Initial values: a(1) = 29012, a(2) = 67356, a(3) = 121593
 */
seq_t a(idx_t n)
{
    if (n == 1) return 29012;
    if (n == 2) return 67356;
    if (n == 3) return 121593;

    seq_t a_nm3 = 29012;   /* a(1)   */
    seq_t a_nm2 = 67356;   /* a(2)   */
    seq_t a_nm1 = 121593;  /* a(3)   */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = 3 * a_nm1;
        ai -= 3 * a_nm2;
        ai += a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = ai;
    }
    return a_nm1;
}
