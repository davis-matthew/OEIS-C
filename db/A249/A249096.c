/*
 * A249096  {2*h^2, h >=1} union {3*k^2, k >=1}, in increasing order.
 * https://oeis.org/A249096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 327, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Merge the two sequences {2*h²} (h ≥ 1) and {3*k²} (k ≥ 1)
 * by a two‑pointer technique.  The n‑th smallest element is returned.
 * This matches the definition of OEIS A249096. */

seq_t a(idx_t n)
{
    /* Sequence indices start at 1.  The caller guarantees n ≥ 1. */
    idx_t h = 1, k = 1;   /* current indices for the two sequences */
    seq_t value = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute the next candidates from each sequence */
        seq_t next_s = (seq_t)2 * ((seq_t)h * (seq_t)h);   /* 2*h² */
        seq_t next_t = (seq_t)3 * ((seq_t)k * (seq_t)k);   /* 3*k² */

        /* Pick the smaller value (the sets are disjoint). */
        if (next_s < next_t) {
            value = next_s;
            ++h;          /* advance the 2*h² sequence */
        } else {
            value = next_t;
            ++k;          /* advance the 3*k² sequence */
        }
    }
    return value;
}
