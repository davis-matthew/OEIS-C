/*
 * A259167  Positive octagonal numbers (A000567) that are squares (A000290)
 *          divided by 2.
 * https://oeis.org/A259167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the Pell equation 2*(3k−1)² – 3m² = 2.
   Let U = 2*(3k−1) and V = m.  Starting solution is U₁=10, V₁=4.
   The recurrence for the valid solutions (those with U ≡ 2 (mod 6)) is
       [U_{i+1}]   [49 120] [U_i]
       [V_{i+1}] = [20  49] [V_i]
   For each i we obtain k_i = (U_i + 2) / 6 and the sequence term
       a(i) = k_i * (3*k_i – 2).
   This generates the OEIS sequence A259167. */

seq_t a(idx_t n)
{
    /* handle offset 1: a(1) is first element */
    seq_t U = (seq_t)10;   /* U_1 */
    seq_t V = (seq_t)4;    /* V_1 */

    /* iterate (n-1) times to reach the nth solution */
    for (idx_t i = 1; i < n; ++i) {
        seq_t new_U = (seq_t)49 * U + (seq_t)120 * V;
        seq_t new_V = (seq_t)20 * U + (seq_t)49 * V;
        U = new_U;
        V = new_V;
    }

    /* k = (U + 2) / 6  (exact division) */
    seq_t k = (U + (seq_t)2) / (seq_t)6;

    /* a = k * (3*k – 2) */
    seq_t a_val = k * ((seq_t)3 * k - (seq_t)2);

    return a_val;
}
