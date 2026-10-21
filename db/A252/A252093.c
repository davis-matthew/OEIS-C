/*
 * A252093  Numbers n such that the pentagonal number P(n) is equal to the sum
 *          of the octagonal numbers N(m), N(m+1) and N(m+2) for some m.
 * https://oeis.org/A252093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A252093: a(n) = 940899*a(n-1) - 940899*a(n-2) + a(n-3)
   offset 1: a(1) = 90, a(2) = 84515202, a(3) = 79520184374490. */
seq_t a(idx_t n)
{
    /* Handle indices below the offset trivially; only values for n>=1 are defined properly. */
    if (n == 0) return 0;

    /* Initial fixed terms (for n = 1, 2, 3). */
    const seq_t a1 = (seq_t)90;
    const seq_t a2 = (seq_t)84515202;
    const seq_t a3 = (seq_t)79520184374490ULL;   /* 64‑bit literal fits in seq_t */

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Recurrence requires the three previous terms.  Use loop over 4..n. */
    seq_t prev3 = a1;   /* a(n-3) */
    seq_t prev2 = a2;   /* a(n-2) */
    seq_t prev1 = a3;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t curr = 940899 * prev1 - 940899 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
