/*
 * A307985  Number of integer-sided triangles with perimeter n and sides a, b,
 *          and c such that a <= b <= c and b|n.
 * https://oeis.org/A307985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n/6) if 3 | n else 0
 * Range:     u8 n <= 1532, u16 n <= 19683, u32 n <= 19683, u64 n <= 19683,
 *            u128 n <= 19683, big any n
 * Validated: standard, pass, 99 terms through n = 8268 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 != 0)
        return 0;
    seq_t k = (seq_t)(n / 3);
    return (k + 1) / 2;
}
