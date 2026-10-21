/*
 * A278579  Quadratic non-residues of 23: numbers n such that Jacobi(n,23) =
 *          -1.
 * https://oeis.org/A278579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+11) = a(n) + 23, a(1)=5, a(2)=7, a(3)=10, a(4)=11, a(5)=14,
 *            a(6)=15, a(7)=17, a(8)=19, a(9)=20, a(10)=21, a(11)=22
 *            (Quadratic non‑residues mod 23)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 11 residues that are quadratic non‑residues modulo 23 */
    static const seq_t residues[11] = {5, 7, 10, 11, 14, 15, 17, 19, 20, 21, 22};

    /* Convert 1‑based index to 0‑based residue index and period count */
    idx_t r = (n - 1) % 11;          /* residue index 0…10 */
    idx_t k = (n - 1) / 11;          /* number of full periods */

    /* a(n) = residue[r] + 23·k */
    return residues[r] + (seq_t)(23 * k);
}
