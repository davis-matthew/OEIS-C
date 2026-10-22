/*
 * A356685  Number of inequivalent simultaneous colorings of the faces,
 *          vertices and edges of the cube under rotational symmetry using at
 *          most n colors.
 * https://oeis.org/A356685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^26 + 9*n^14 + 8*n^10 + 6*n^8) / 24 (OEIS A356685)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is outside the OEIS offset – return 0 for safety */
    if (n == 0) return 0;

#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit integer arithmetic for builtin types. */
    unsigned __int128 nn = (unsigned __int128)n;

    unsigned __int128 p8  = nn;
    for (int i = 1; i < 8;  ++i) p8  *= nn;
    unsigned __int128 p10 = p8;
    for (int i = 8; i < 10; ++i) p10 *= nn;
    unsigned __int128 p14 = p10;
    for (int i = 10; i < 14; ++i) p14 *= nn;
    unsigned __int128 p26 = p14;
    for (int i = 14; i < 26; ++i) p26 *= nn;

    unsigned __int128 sum = p26
                         + (unsigned __int128)9 * p14
                         + (unsigned __int128)8 * p10
                         + (unsigned __int128)6 * p8;

    sum /= 24;                 /* division is exact */
    return (seq_t)sum;
#else
    /* Fallback for arbitrary‑precision builds: use helper pow routine. */
    seq_t term8  = pow_uint((uint64_t)n, 8);
    seq_t term10 = pow_uint((uint64_t)n, 10);
    seq_t term14 = pow_uint((uint64_t)n, 14);
    seq_t term26 = pow_uint((uint64_t)n, 26);

    seq_t sum = term26
              + (seq_t)9 * term14
              + (seq_t)8 * term10
              + (seq_t)6 * term8;

    return sum / (seq_t)24;
#endif
}
