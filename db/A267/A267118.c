/*
 * A267118  Lee Sallows's 3 X 3 semimagic square of squares, read by rows.
 * https://oeis.org/A267118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabf
 * Formula:   a(n) = base[(n-1)%9] * ( ((n-1)/9 + 1)² ) where base =
 *            {16129,2116,3364,4,12769,8836,5476,6724,9409}
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* 0 is undefined, return 0 */

    static const seq_t base[9] = {(seq_t)16129, (seq_t)2116, (seq_t)3364,
                                  (seq_t)4,    (seq_t)12769, (seq_t)8836,
                                  (seq_t)5476, (seq_t)6724,  (seq_t)9409};

    idx_t block = (n - 1) / 9;        /* which scaled square (0‑based)   */
    idx_t pos   = (n - 1) % 9;        /* position within the square      */
    seq_t k     = block + 1;          /* scaling factor                */
    seq_t scale = (seq_t)k * (seq_t)k;/* k² (as seq_t)                  */

    return base[pos] * scale;         /* result in seq_t                */
}
