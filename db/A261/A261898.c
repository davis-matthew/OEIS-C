/*
 * A261898  Values of |G-hat_1(n)|, a sum involving Stirling numbers of the
 *          second kind.
 * https://oeis.org/A261898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2) * (2n-1)!! / 3 for n > 0; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        result *= (seq_t)(2 * k - 1);      /* multiply by odd factors (2k‑1) */
    }
    result *= (seq_t)(n + 2);             /* multiply by (n+2)   */
    result /= 3;                          /* division by 3, exact */
    return result;
}
