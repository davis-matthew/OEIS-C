/*
 * A313064  Coordination sequence Gal.6.195.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, * a(n) = 1 + 3 + Σ_{i=0}^{n-2} c[i mod 12] *
 *            where c = {5,5,4,5,6,6,5,4,5,5,4,4} (the period of successive
 *            differences)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Number of differences from a(1) to a(n) is n.
       We handle the first difference (a(1)-a(0)=3) separately. */
    idx_t m = n - 1;          /* equals number of terms in the cycle to add */
    seq_t result = (seq_t)1;  /* a(0) */
    result += (seq_t)3;       /* d(1) = a(1)-a(0) */

    /* Define the 12‑term cycle of differences for n>=2. */
    static const seq_t cycle[12] = {
        (seq_t)5, (seq_t)5, (seq_t)4, (seq_t)5,
        (seq_t)6, (seq_t)6, (seq_t)5, (seq_t)4,
        (seq_t)5, (seq_t)5, (seq_t)4, (seq_t)4
    };

    /* Prefix sums of the cycle for quick partial‐sum queries. */
    static const seq_t prefix[13] = {
        (seq_t)0, (seq_t)5,  (seq_t)10, (seq_t)14,
        (seq_t)19, (seq_t)25, (seq_t)31, (seq_t)36,
        (seq_t)40, (seq_t)45, (seq_t)50, (seq_t)54,
        (seq_t)58
    };

    /* Compute how many full cycles fit into the first (n-1) differences. */
    idx_t full = m / 12U;
    idx_t rem  = m % 12U;

    /* Sum contribution of complete cycles. */
    result += (seq_t)full * (seq_t)58;

    /* Add the partial cycle (first 'rem' elements). */
    result += prefix[rem];

    return result;
}
