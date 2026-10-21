/*
 * A276164  a(n) is the maximum first-player score of a "Coins in a Row" game
 *          over all permutations of coins 1..n with both players using a
 *          minimax strategy.
 * https://oeis.org/A276164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = (A000217(n) + A276163(n)) / 2 * where A000217(n) = n *
 *            (n+1) / 2 * A276163(n) = { (n/2)^2 , if n is even * n , if n ≡ 1
 *            (mod 4) * n - 1 , if n ≡ 3 (mod 4) }
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* triangular number A000217(n) */
    seq_t tn = (seq_t)n;
    seq_t triangular = tn * (tn + 1) / 2;

    /* compute A276163(n) */
    seq_t A;
    if ((n & 1) == 0) {                       /* even n */
        idx_t k = n / 2;
        A = (seq_t)k * (seq_t)k;              /* k^2 */
    } else if ((n & 3) == 1) {                /* n ≡ 1 (mod 4) */
        A = tn;                               /* n */
    } else {                                  /* n ≡ 3 (mod 4) */
        A = tn - 1;                           /* n - 1 */
    }

    /* final a(n) */
    return (triangular + A) / 2;
}
