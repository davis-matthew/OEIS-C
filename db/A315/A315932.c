/*
 * A315932  Coordination sequence Gal.4.110.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let k = n / 6, r = n % 6. a(6k) = 1 if k==0 = 34*k - 2 otherwise
 *            a(6k+1) = 6 if k==0 = 34 if k==1 = 28*k + 8 otherwise a(6k+2) =
 *            29*k + 8 a(6k+3) = 32*k + 16 a(6k+4) = 29*k + 19 a(6k+5) = 22 if
 *            k==0 = 28*k + 24 otherwise
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 6;
    idx_t r = n % 6;
    switch (r) {
    case 0: /* a(6k) */
        return k == 0 ? (seq_t)1 : (seq_t)(34 * k - 2);
    case 1: /* a(6k+1) */
        if (k == 0) return (seq_t)6;
        if (k == 1) return (seq_t)34;
        return (seq_t)(28 * k + 8);
    case 2: /* a(6k+2) */
        return (seq_t)(29 * k + 8);
    case 3: /* a(6k+3) */
        return (seq_t)(32 * k + 16);
    case 4: /* a(6k+4) */
        return (seq_t)(29 * k + 19);
    case 5: /* a(6k+5) */
        return k == 0 ? (seq_t)22 : (seq_t)(28 * k + 24);
    }
    /* unreachable, but return 0 to silence compiler warnings */
    return (seq_t)0;
}
