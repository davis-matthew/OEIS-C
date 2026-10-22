/*
 * A376538  Natural numbers whose iterated squaring modulo 1000 eventually
 *          settles at the attractor 1.
 * https://oeis.org/A376538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS: A376538 – Natural numbers whose iterated squaring modulo 1000
   eventually settles at the attractor 1.
   The set of admissible residues modulo 1000 is
        {1, 57, 193, 249, 251, 307, 443, 499,
         501, 557, 693, 749, 751, 807, 943, 999}
   Each such residue repeats every 1000.  Therefore the sequence is
   arithmetic in blocks of 16 values:
        a(n) = r[(n‑1) mod 16] + 1000 · ⌊(n‑1)/16⌋
*/

#include <stdbool.h>

static const seq_t residues[16] = {
    1ULL, 57ULL, 193ULL, 249ULL, 251ULL, 307ULL,
    443ULL, 499ULL, 501ULL, 557ULL, 693ULL, 749ULL,
    751ULL, 807ULL, 943ULL, 999ULL
};

seq_t a(idx_t n)
{
    /* n >= 1; use unsigned arithmetic */
    idx_t n1 = n - 1;                 /* zero‑based index */
    idx_t q   = n1 / 16;              /* block number */
    idx_t r   = n1 % 16;              /* position inside block */

    /* compute result in seq_t */
    seq_t term = (seq_t)residues[r] + 1000ULL * (seq_t)q;
    return term;
}
