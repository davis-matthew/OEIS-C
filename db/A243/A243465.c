/*
 * A243465  Number of ways 3 domicules can be placed on an n X n square.
 * https://oeis.org/A243465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2574-2328*n-144*n^5-96*n^4+1188*n^3-854*n^2+32*n^6)/3
 *            for n>=3, a(n)=0 for n<3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 27, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* Use 128‑bit arithmetic internally to avoid overflow when seq_t is 64‑bit. */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 n2   = nn * nn;
    unsigned __int128 n3   = n2 * nn;
    unsigned __int128 n4   = n3 * nn;
    unsigned __int128 n5   = n4 * nn;
    unsigned __int128 n6   = n5 * nn;

    unsigned __int128 num =
        32ULL * n6
        - 144ULL * n5
        - 96ULL * n4
        + 1188ULL * n3
        - 854ULL * n2
        - 2328ULL * nn
        + 2574ULL;

    return (seq_t)(num / 3ULL);
}
