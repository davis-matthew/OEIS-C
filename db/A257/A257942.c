/*
 * A257942  a(n) = (n+1)*(n+2)/A014695(n+1), where A014695 is repeat (1, 2, 2,
 *          1).
 * https://oeis.org/A257942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)/A014695(n+1) * A014695 repeats 1,2,2,1. The
 *            following case‑separated * implementation uses the equivalent
 *            period‑4 formulas: * a(4k) = (2k+1)*(4k+1) * a(4k+1) =
 *            (2k+1)*(4k+3) * a(4k+2) = (4k+3)*(4k+4) * a(4k+3) =
 *            (4k+4)*(4k+5)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k_idx = n >> 2;          /* k = floor(n/4) */
    seq_t k     = (seq_t)k_idx;    /* cast to seq_t for arithmetic */
    switch (n & 3) {
    case 0:   /* n ≡ 0 mod 4 */
        return ((seq_t)2 * k + 1) * ((seq_t)4 * k + 1);
    case 1:   /* n ≡ 1 mod 4 */
        return ((seq_t)2 * k + 1) * ((seq_t)4 * k + 3);
    case 2:   /* n ≡ 2 mod 4 */
        return ((seq_t)4 * k + 3) * ((seq_t)4 * k + 4);
    case 3:   /* n ≡ 3 mod 4 */
        return ((seq_t)4 * k + 4) * ((seq_t)4 * k + 5);
    }
    return 0; /* unreachable */
}
