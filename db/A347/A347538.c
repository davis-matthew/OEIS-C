/*
 * A347538  Number of minimum dominating sets in the cycle graph C_n.
 * https://oeis.org/A347538
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 if n == 0 (mod 3), * n*(n+5)/6 if n == 1 (mod 3), * n
 *            if n == 2 (mod 3); (offset 3)
 * Range:     u8 n <= 36, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r;
    switch (n % 3U) {
    case 0U:
        r = 3;
        break;
    case 1U:
        r = ((seq_t)n * ((seq_t)n + 5)) / 6;
        break;
    default: /* case 2U */
        r = (seq_t)n;
        break;
    }
    return r;
}
