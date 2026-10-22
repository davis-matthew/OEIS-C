/*
 * A327582  a(n) = (17 * 7^(2*n+1) + 1)/24. Sequence related to the properties
 *          of the partition function A000041 modulo a power of 7.
 * https://oeis.org/A327582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (17 * 7^(2*n+1) + 1) / 24
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=5, a(1)=243, a(n)=50*a(n-1)-49*a(n-2) for n>1 */

seq_t a(idx_t n) {
    static const seq_t a0 = (seq_t)5;
    static const seq_t a1 = (seq_t)243;

    if (n == 0) return a0;
    if (n == 1) return a1;

    seq_t prev = a0;
    seq_t cur  = a1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = ((seq_t)50 * cur) - ((seq_t)49 * prev);
        prev = cur;
        cur  = next;
    }
    return cur;
}
