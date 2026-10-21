/*
 * A283678  Number of possible draws of 2n pairs of consecutive cards from a
 *          set of 4n + 1 cards, so that the card that initially occupies the
 *          central position is not selected.
 * https://oeis.org/A283678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n, n) * ((2n-1)!!)^2 Recurrence: a(0) = 1, a(n)
 *            = 2*(2n-1)^3 / n * a(n-1) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of sequence A283678.
 * Uses only integer arithmetic on the abstract type seq_t,
 * which may be an arbitrary‑precision integer type in C++ mode
 * or an unsigned integer type in C mode. */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t res = (seq_t)1;           /* holds a(k-1) during the loop */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute (2k-1) as seq_t */
        seq_t odd = (seq_t)2 * (seq_t)k - (seq_t)1;

        /* cube = (2k-1)^3 */
        seq_t cube = odd * odd * odd;

        /* factor = 2 * (2k-1)^3 */
        seq_t factor = (seq_t)2 * cube;

        /* res = a(k) = factor * a(k-1) / k  (exact division) */
        res = (res * factor) / (seq_t)k;
    }

    return res;
}
