/*
 * A396468  a(n) is the number of distinct triples of triangles chosen from
 *          n^2 unit triangles of a triangular grid up to rotation and triples
 *          in which all three triangles lie in the same row parallel to a
 *          side of the grid.
 * https://oeis.org/A396468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A396285(n) – A006324(n‑1) A396285(n) = (C(n², 3) +
 *            2·Fix_rot(n)) / 3 Fix_rot(n) = (n² – (n² mod 3)) / 3 A006324(k)
 *            = 1 + 10·(k–1) + 25·C(k–1, 2) + 24·C(k–1, 3) + 8·C(k–1, 4)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom_small_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_small_idx.h"

seq_t a(idx_t n)
{
    /*  offset 1 */
    if (n < 2) return 0;

    idx_t N = n * n;                    /* total number of unit triangles          */
    seq_t total = (seq_t)N * ((seq_t)N - 1) * ((seq_t)N - 2) / 6;   /* C(N,3) */

    idx_t mod3 = N % 3;
    seq_t fixrot = ((seq_t)N - (seq_t)mod3) / 3;            /* triples fixed by 120° rotation */

    seq_t orbits = (total + 2 * fixrot) / 3;                /* A396285(n)  */

    idx_t t = n - 2;      /* t = n‑2, used for A006324(n‑1)         */
    seq_t same_row = 1;                        /* start value from the formula          */
    same_row += (seq_t)10 * t;
    if (t >= 2) same_row += (seq_t)25 * binom_small_idx(t, 2);
    if (t >= 3) same_row += (seq_t)24 * binom_small_idx(t, 3);
    if (t >= 4) same_row += (seq_t)8  * binom_small_idx(t, 4);

    return orbits - same_row;                  /* final a(n)  */
}
