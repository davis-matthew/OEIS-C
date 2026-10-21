/*
 * A269819  Numbers that are congruent to {5, 11, 13, 19} mod 24.
 * https://oeis.org/A269819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = offset[(n-1)%4] + 24 * floor((n-1)/4)
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  offset = {5, 11, 13, 19}  */

seq_t a(idx_t n)
{
    /* indexes start at 1 */
    idx_t idx   = n - 1U;          /* safe for n >= 1 */
    idx_t block = idx / 4U;        /* number of full 24‑step blocks */
    idx_t pos   = idx % 4U;        /* position in the block */

    /* static array of the initial four residues */
    static const seq_t offset[4] = {5, 11, 13, 19};

    /* cast to seq_t to ensure the multiplication stays in seq_t space */
    return offset[pos] + (seq_t)24U * (seq_t)block;
}
