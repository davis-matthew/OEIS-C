/*
 * A313926  Coordination sequence Gal.4.140.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5; for n>=2, a(n)=5 + 44*floor((n-1)/8) +
 *            {0,5,11,17,23,29,34,39}[(n-1)%8]
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;
    if (n == 1ULL)
        return (seq_t)5;

    /* At this point n >= 2 */
    idx_t m = n - 1;                     /* increments after a(1)       */
    idx_t full = m / 8;                  /* complete 8‑step cycles      */
    idx_t rem  = m % 8;                  /* remaining increments in cycle */

    /* Prefix sums of the 8‑step pattern [5,6,6,6,6,5,5,5]    */
    static const seq_t pref[9] = {
        0ULL, 5ULL, 11ULL, 17ULL,
        23ULL, 29ULL, 34ULL, 39ULL, 44ULL
    };

    seq_t result = (seq_t)5;              /* value a(1)                  */
    result += (seq_t)full * (seq_t)44;    /* add full cycles             */
    result += pref[rem];                  /* add partial cycle           */
    return result;
}
