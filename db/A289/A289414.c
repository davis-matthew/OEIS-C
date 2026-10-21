/*
 * A289414  a(n) = ((10-sqrt(10))^n + (10+sqrt(10))^n) / 2.
 * https://oeis.org/A289414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((10-sqrt(10))^n + (10+sqrt(10))^n) / 2 Recurrence:
 *            a(0)=1, a(1)=10, a(n)=20*a(n-1)-90*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    /* Two constants expressed as seq_t so that multiplication is
       handled generically for both built‑in and big‑integer types. */
    const seq_t twenty = 20;
    const seq_t ninety = 90;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)10;  /* a(1) */
    seq_t cur;

    /* Iterate from n = 2 up to the requested index. */
    for (idx_t i = 2; i <= n; ++i) {
        cur = (prev1 * twenty) - (prev2 * ninety);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
