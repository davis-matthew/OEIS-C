/*
 * A283997  a(n) = n XOR A005187(floor(n/2)), where XOR is bitwise-xor
 *          (A003987).
 * https://oeis.org/A283997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, hear
 * Formula:   a(n) = n XOR ((2*floor(n/2)) - popcount(floor(n/2)))
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n XOR A005187(floor(n/2)) */
/* Here A005187(m) = 2*m - popcount(m)   (popcount counts ones in binary) */

seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    const idx_t k = n >> 1;
    /* popcount of k (depends on compiler, works for 64‑bit types) */
    const unsigned int pc = __builtin_popcountll(k);
    /* compute A005187(k) = 2*k - pc  in type seq_t */
    const seq_t g = ((seq_t)k << 1) - (seq_t)pc;
    /* final value */
    return (seq_t)n ^ g;
}
