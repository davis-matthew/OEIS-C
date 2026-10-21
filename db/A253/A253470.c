/*
 * A253470  Indices of centered triangular numbers (A005448) which are also
 *          centered pentagonal numbers (A005891).
 * https://oeis.org/A253470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1)-9*a(n-2)+a(n-3) (a(1)=1, a(2)=5, a(3)=36)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */
    seq_t a0 = 1;                  /* a(1) */
    if (n == 1) return a0;
    seq_t a1 = 5;                  /* a(2) */
    if (n == 2) return a1;
    seq_t a2 = 36;                 /* a(3) */
    if (n == 3) return a2;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = ((seq_t)9) * a2 - ((seq_t)9) * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
