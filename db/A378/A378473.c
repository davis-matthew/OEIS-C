/*
 * A378473  The number of n-colorings of the vertices of the truncated
 *          octahedron up to rotation and reflection.
 * https://oeis.org/A378473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/48)*(n^24 + 3*n^16 + 16*n^12 + 8*n^8 + 12*n^6 + 8*n^4)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit temporaries for integer seq_t types of size ≤ 8
       to avoid overflow; otherwise assume seq_t itself can handle the
       intermediate products (e.g. an arbitrary‑precision type). */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 n2 = N * N;
        unsigned __int128 n4 = n2 * n2;
        unsigned __int128 n6 = n4 * n2;
        unsigned __int128 n8 = n4 * n4;
        unsigned __int128 n12 = n6 * n6;
        unsigned __int128 n16 = n8 * n8;
        unsigned __int128 n24 = n12 * n12;
        unsigned __int128 sum = n24 +
                                (unsigned __int128)3 * n16 +
                                (unsigned __int128)16 * n12 +
                                (unsigned __int128)8 * n8 +
                                (unsigned __int128)12 * n6 +
                                (unsigned __int128)8 * n4;
        return (seq_t)(sum / (unsigned __int128)48);
    } else {
        /* seq_t is an arbitrary‑precision type that can hold the
           intermediate results directly.  Compute powers with repeated
           multiplication. */
        seq_t N = (seq_t)n;
        seq_t n2 = N * N;
        seq_t n4 = n2 * n2;
        seq_t n6 = n4 * n2;
        seq_t n8 = n4 * n4;
        seq_t n12 = n6 * n6;
        seq_t n16 = n8 * n8;
        seq_t n24 = n12 * n12;
        seq_t sum = n24 + (seq_t)3 * n16 + (seq_t)16 * n12 +
                    (seq_t)8 * n8 + (seq_t)12 * n6 + (seq_t)8 * n4;
        return sum / (seq_t)48;
    }
}
