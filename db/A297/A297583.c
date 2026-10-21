/*
 * A297583  Number of 2 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 or 4 neighboring 1s.
 * https://oeis.org/A297583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-3) + 2*a(n-4) initial values: a(1)=1,
 *            a(2)=5, a(3)=11, a(4)=17
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, though n ≥ 1 is expected */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)17;

    seq_t prev1 = (seq_t)17; /* a(4)   */
    seq_t prev2 = (seq_t)11; /* a(3)   */
    seq_t prev3 = (seq_t)5;  /* a(2)   */
    seq_t prev4 = (seq_t)1;  /* a(1)   */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t current = prev1 + 4 * prev3 + 2 * prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
