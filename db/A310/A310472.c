/*
 * A310472  Coordination sequence Gal.6.347.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n >= 1 let k = (n-1)/12, r = (n-1)%12. Then a(n) =
 *            base[r] + 62*k, where base =
 *            {4,10,15,21,26,31,36,41,47,52,58,62}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function relies only on integer types and basic arithmetic. */
seq_t a(idx_t n)
{
    /* Base values for indices 1..12 (0-based array indices 0..11). */
    static const seq_t base[12] = {
        (seq_t)4,  (seq_t)10, (seq_t)15, (seq_t)21,
        (seq_t)26, (seq_t)31, (seq_t)36, (seq_t)41,
        (seq_t)47, (seq_t)52, (seq_t)58, (seq_t)62
    };

    if (n == 0) return (seq_t)1;

    idx_t k = (n - 1) / 12;
    idx_t r = (n - 1) % 12;

    /* Compute result using integer arithmetic. */
    seq_t result = base[r] + (seq_t)62 * (seq_t)k;
    return result;
}
