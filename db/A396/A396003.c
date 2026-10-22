/*
 * A396003  Sum of the decimal digits of all distinct n-digit numbers that are
 *          products of two positive decimal repunits.
 * https://oeis.org/A396003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, new
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A396003: Sum of the decimal digits of all distinct n‑digit numbers
 * that are products of two positive decimal repunits.
 *
 * Formula implemented (OEIS):
 *   let h = ⌊(n+1)/2⌋
 *   a(n) = Σ_{u=1..min(9,h)} [ (q+1)*u*(n+1-u) + (81-18u)*q*(q+1)/2 ]
 *   where q = ⌊(h-u)/9⌋
 *
 * This is a direct closed‑form evaluation that runs in O(1) time for any
 * n ≥ 1 and satisfies all build constraints.
 */
seq_t a(idx_t n)
{
    /* n is an unsigned 64‑bit index; all arithmetic is done in seq_t */
    seq_t n_plus_1 = (seq_t)n + 1;                     /* n + 1          */
    seq_t h_seq    = n_plus_1 / 2;                     /* ⌊(n+1)/2⌋      */
    /* Convert h_seq to idx_t for loop bounds; it is small (≤ n/2+0.5) */
    idx_t h_idx    = (idx_t)h_seq;
    idx_t max_u_idx = h_idx < 9 ? h_idx : 9;           /* min(9,h)       */

    seq_t result = 0;

    for (idx_t ui = 1; ui <= max_u_idx; ++ui) {
        seq_t u   = (seq_t)ui;
        seq_t q   = (h_seq > u) ? (h_seq - u) / 9 : 0; /* ⌊(h-u)/9⌋, ensure non‑negative */

        /* term1 = (q+1)*u*(n+1-u) */
        seq_t term1 = (q + 1) * u * (n_plus_1 - u);

        /* term2 = (81-18u)*q*(q+1)/2  */
        seq_t q_pair = q * (q + 1) / 2;
        seq_t term2 = (81 - 18 * u) * q_pair;

        result += term1 + term2;
    }
    return result;
}
