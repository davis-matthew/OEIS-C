/*
 * A340536  Digital root of 2*n^2.
 * https://oeis.org/A340536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root(2*n^2) (OEIS A340536)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 9 first to avoid overflow. */
    seq_t n_mod = (seq_t)(n % 9);          /* n modulo 9, fits in seq_t safely */
    /* Compute (n^2) modulo 9 */
    seq_t sq_mod = (n_mod * n_mod) % 9;    /* 0..8 */
    /* Multiply by 2 and reduce modulo 9 */
    seq_t val_mod = (sq_mod * 2) % 9;      /* 0..8 */

    /* Digital root of x: if x % 9 == 0 then 9 else x % 9 (for x>0) */
    return (val_mod == 0) ? 9 : val_mod;
}
