/*
 * A361609  a(n) = 4^n*(1 + (23/8)*n + (9/8)*n^2).
 * https://oeis.org/A361609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(1 + (23/8)*n + (9/8)*n^2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 12*a(n-1) - 48*a(n-2) + 64*a(n-3) */
/* Initial values: a(0)=1, a(1)=20, a(2)=180 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1u;
    if (n == 1u) return (seq_t)20u;
    if (n == 2u) return (seq_t)180u;

    seq_t a0 = (seq_t)1u;   /* a(n-3)   */
    seq_t a1 = (seq_t)20u;  /* a(n-2)   */
    seq_t a2 = (seq_t)180u; /* a(n-1)   */

    for (idx_t i = 3u; i <= n; ++i) {
        seq_t next = (seq_t)((seq_t)12u * a2 - (seq_t)48u * a1 + (seq_t)64u * a0);
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
