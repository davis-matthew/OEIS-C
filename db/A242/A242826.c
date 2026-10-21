/*
 * A242826  Primes formed by the initial digits of the decimal expansion of
 *          1/13, starting at the first nonzero digit in the expansion.
 * https://oeis.org/A242826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   * a(1) = 7 * a(2) = 769 * For n ≥ 3: a(n) is the integer made
 *            from (2n−5) full 6‑digit blocks * "769230" followed by the
 *            3‑digit suffix "769". * That is, a(n) uses the first (12n−27)
 *            digits of the decimal expansion of 1/13
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* Not defined for n=0, return 0 safely */
    if (n == 1) return 7;
    if (n == 2) return 769;

    /* Number of full 6‑digit blocks '769230' to append before the final '769' */
    idx_t blocks = 2 * n - 5;

    seq_t res = 0;

    for (idx_t i = 0; i < blocks; ++i) {
        res = res * 1000000 + 769230;  /* Append one 6‑digit block */
    }

    /* Append the final 3‑digit suffix */
    res = res * 1000 + 769;
    return res;
}
