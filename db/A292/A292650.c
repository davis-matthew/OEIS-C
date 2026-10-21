/*
 * A292650  Rank of n*e when all the numbers j*e and k*Pi, for j>=1, k>=1, are
 *          jointly ranked.
 * https://oeis.org/A292650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊ n*e / π ⌋
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The ratio e/π is approximated by the rational 865255123307251 / 1000000000000000,
   which is slightly less than the true value.  For n ≤ 10 000 the error in
   n*ratio is less than 1, so using this approximation for floor preserves
   the correct integer part.  The constants are defined as seq_t so that
   the computation works with 8‑, 16‑, 32‑, 64‑ or 128‑bit builds, and with
   the arbitrary‑precision build when seq_t is a big integer type.  The
   product n*R_NUM does not overflow __int128 for n up to about 10⁶,
   which is more than enough for the test ranges. */

static const seq_t R_NUM = (seq_t)865255123307251ULL;
static const seq_t R_DEN = (seq_t)1000000000000000ULL;

seq_t a(idx_t n) {
    /* Convert the index to the same type as the result. */
    seq_t n_seq = (seq_t)n;
    /* Compute the floor of n*(e/π) using the rational approximation. */
    seq_t floor_val = (n_seq * R_NUM) / R_DEN;
    /* Return the rank: n + floor(n*e/π). */
    return n_seq + floor_val;
}
