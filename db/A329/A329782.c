/*
 * A329782  Numbers that are congruent to {+-1, +-4, +-5, +-6, +-7, +-9, +-10,
 *          +-11, +-13, +-15, +-16, +-19} (mod 40).
 * https://oeis.org/A329782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40 * floor((n-1)/24) + r[(n-1) mod 24] with r =
 *            (1,4,5,6,7,9,10,11,13,15,16,19,21,24,25,27,29,30,31,33,34,35,36,39)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; values for n<1 are irrelevant */
        return 0;

    /* Sorted residues of numbers ≡ ±1, ±4, ±5, ±6, ±7, ±9, ±10,
       ±11, ±13, ±15, ±16, ±19 (mod 40) */
    static const seq_t r[24] = {
        1, 4, 5, 6, 7, 9, 10, 11,
        13, 15, 16, 19, 21, 24, 25, 27,
        29, 30, 31, 33, 34, 35, 36, 39
    };

    idx_t idx   = n - 1;          /* convert to 0‑based index   */
    idx_t block = idx / 24;       /* which 40‑block              */
    idx_t pos   = idx % 24;       /* position within the block   */

    /* Compute 40*block + residue[pos] */
    return (seq_t)block * 40 + r[pos];
}
