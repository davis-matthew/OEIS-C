/*
 * A322271  Smallest multiplication factors f, prime or 1, for all b (mod 24),
 *          coprime to 24, so that b*f is a nonzero square mod 8 and mod 3.
 * https://oeis.org/A322271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n‑th integer coprime to 24, i.e. the minimal *
 *            multiplicative factor f (prime or 1) such that b_n * f is a *
 *            nonzero square modulo 8 and modulo 3. Because every residue *
 *            coprime to 24 is either a square modulo 8 and 3 itself (f = 1) *
 *            or otherwise requires f = b, the sequence is exact...
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */

seq_t a(idx_t n)
{
    /* residues modulo 24 that are coprime to 24 */
    static const seq_t residues[8] = {
        (seq_t)1,
        (seq_t)5,
        (seq_t)7,
        (seq_t)11,
        (seq_t)13,
        (seq_t)17,
        (seq_t)19,
        (seq_t)23
    };

    /* zero‑based position of n */
    idx_t pos = n - 1u;                  /* n ≥ 1 by contract */
    seq_t base   = residues[(size_t)(pos % 8u)];
    seq_t cycle  = (seq_t)24u * (pos / 8u);
    return base + cycle;
}
