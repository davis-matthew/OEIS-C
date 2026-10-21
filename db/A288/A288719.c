/*
 * A288719  Detour index of the n-triangular grid graph.
 * https://oeis.org/A288719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 9*n^5 + 29*n^4 + 39*n^3 + 18*n^2 - 48)/16 for n >
 *            3
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases for the sequence */
    if (n == 0) return 0;
    if (n == 1) return 6;
    if (n == 2) return 69;
    if (n == 3) return 399;

    /* For n > 3 use: v = (n+1)(n+2)/2, a(n) = v*(v-1)^2/2 - 3 */

#if defined(__SIZEOF_INT128__)
    /* If seq_t is 64‑bit (or smaller), use 128‑bit intermediates to avoid overflow */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 n128 = n;                         /* cast to 128 bits */
        unsigned __int128 v   = (n128 + 1u) * (n128 + 2u) / 2u;
        unsigned __int128 vm1 = v - 1u;
        unsigned __int128 val = v * vm1 * vm1 / 2u - 3u;
        return (seq_t)val;                                   /* truncate to seq_t */
    }
    /* seq_t is larger (e.g., arbitrary precision); use seq_t operations */
#endif

    /* Default path: compute with seq_t arithmetic (arbitrary precision or 128‑bit) */
    seq_t v   = ((seq_t)(n + 1u)) * ((seq_t)(n + 2u)) / 2u;
    seq_t vm1 = v - 1u;
    seq_t tmp = v * vm1 * vm1 / 2u;
    return tmp - 3u;
}
