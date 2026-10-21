/*
 * A264339  Number of (n+1)X(6+1) arrays of permutations of 0..n*7+6 with each
 *          element having index change +-(.,.) 0,0 0,1 or 1,2.
 * https://oeis.org/A264339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 87*a(n-1) + 374*a(n-2) - 470*a(n-3) + 207*a(n-4) +
 *            3*a(n-5) Initial values: a(1)=2521, a(2)=223356, a(3)=20386617,
 *            a(4)=1855980772, a(5)=168990466353
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2521;
    if (n == 2) return (seq_t)223356;
    if (n == 3) return (seq_t)20386617;
    if (n == 4) return (seq_t)1855980772;
    if (n == 5) return (seq_t)168990466353;

    seq_t a1 = (seq_t)2521;
    seq_t a2 = (seq_t)223356;
    seq_t a3 = (seq_t)20386617;
    seq_t a4 = (seq_t)1855980772;
    seq_t a5 = (seq_t)168990466353;

    for (idx_t i = 6; i <= n; ++i) {
        seq_t term1 = 87 * a5 + 374 * a4 + 207 * a2 + 3 * a1;
        seq_t next  = term1 - 470 * a3;   /* term1 ≥ 470*a3 for all n≥6 */
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = next;
    }
    return a5;
}
