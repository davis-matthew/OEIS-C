/*
 * A249886  Number of length 3+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (84*n^9 + 108*n^8 + 400*n^7 + 560*n^6 + 826*n^5 + *
 *            637*n^4 + 140*n^3 + 165*n^2 + 20*n) / 84
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula is valid for all n ≥ 1.  It is computed using 128‑bit
 * intermediate arithmetic to avoid overflow on 64‑bit seq_t builds. */

seq_t a(idx_t n)
{
    if (n == 0)               /* sequence offset is 1, guard against 0 */
        return 0;

    /* Use 128‑bit integer for intermediate calculations */
    unsigned __int128 sn   = (unsigned __int128)n;
    unsigned __int128 n2   = sn * sn;
    unsigned __int128 n3   = n2 * sn;
    unsigned __int128 n4   = n3 * sn;
    unsigned __int128 n5   = n4 * sn;
    unsigned __int128 n6   = n5 * sn;
    unsigned __int128 n7   = n6 * sn;
    unsigned __int128 n8   = n7 * sn;
    unsigned __int128 n9   = n8 * sn;

    unsigned __int128 total = 0;
    total += (unsigned __int128)84  * n9;
    total += (unsigned __int128)108  * n8;
    total += (unsigned __int128)400  * n7;
    total += (unsigned __int128)560  * n6;
    total += (unsigned __int128)826  * n5;
    total += (unsigned __int128)637  * n4;
    total += (unsigned __int128)140  * n3;
    total += (unsigned __int128)165  * n2;
    total += (unsigned __int128)20   * sn;

    unsigned __int128 result128 = total / (unsigned __int128)84;
    return (seq_t)result128;
}
