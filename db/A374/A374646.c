/*
 * A374646  Paradiddle version of Thue-Morse sequence.
 * https://oeis.org/A374646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A160381(n)+1 mod 2
 * Range:     u8 n <= 9586, u16 n <= 9586, u32 n <= 9586, u64 n <= 9586,
 *            u128 n <= 9586, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the kth term of the Paradiddle version of Thue-Morse sequence.
   The sequence is the limit of repeatedly applying the morphism:
      0 → 0 1 0 0
      1 → 1 0 1 1
   starting from the axiom 1 0 1 1.  */
seq_t a(idx_t n)
{
    /* base axiom: 1 0 1 1 */
    if (n < 4u) {
        static const seq_t axiom[4] = {1, 0, 1, 1};
        return axiom[(idx_t)n];
    }

    /* Recursively find the parent symbol */
    idx_t parent_idx = n >> 2;         /* division by 4 */
    idx_t r          = n & 3u;         /* remainder modulo 4 */

    seq_t parent = a(parent_idx);

    /* Images of 0 and 1 under the morphism.  */
    if (parent == 0) {
        static const seq_t img0[4] = {0, 1, 0, 0};
        return img0[(idx_t)r];
    } else {
        static const seq_t img1[4] = {1, 0, 1, 1};
        return img1[(idx_t)r];
    }
}
