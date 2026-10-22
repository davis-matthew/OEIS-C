/*
 * A349595  Number of self-counting sequences of length n (sequences indexed
 *          from 0 to (n-1) where a(i) is the number of times i appears in the
 *          sequence).
 * https://oeis.org/A349595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1,2,3; 2 for n = 4; 1 for n = 5; 0 for n = 6; 1
 *            for all n > 6 (exact number of self‑counting sequences)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2 || n == 3) return 0;
    if (n == 4) return 2;
    if (n == 5) return 1;
    if (n == 6) return 0;
    return 1;
}
