/*
 * A315506  Coordination sequence Gal.4.103.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; * For n ≥ 1: * a(n) = 1 + 5 + 60 * floor((n-1)/11) +
 *            Σ_{i=0}^{(n-1)%11-1} p[i] * where p = {5,6,3,5,10,5,3,6,5,6,6}
 *            are the periodic increments * of the sequence after the first
 *            term. * This yields the OEIS sequence A315506
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t P[11] = {
    5, 6, 3, 5, 10, 5, 3, 6, 5, 6, 6
};

/* Prefix sums of P (ps[0] = 0) */
static const seq_t PS[12] = {
    0,   5,  11,  14,  19,  29,  34,  37,  43,  48,  54,  60
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    /* Start with a(0) and the first increment d0 = 5 */
    seq_t result = 1 + 5;

    /* Remaining increments start from d1 and follow the pattern P */
    idx_t remaining = n - 1;          /* number of diffs after d0 */
    idx_t cycles    = remaining / 11; /* full pattern cycles */
    idx_t rem       = remaining % 11; /* partial cycle */

    /* Add contributions from full cycles */
    result += (seq_t)cycles * PS[11];

    /* Add partial cycle using prefix sums */
    result += PS[rem];

    return result;
}
