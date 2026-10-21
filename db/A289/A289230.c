/*
 * A289230  Number of nonequivalent ways to select 3 disjoint point triples
 *          from an n X n X n triangular point grid, each point triple forming
 *          a 2 X 2 X 2 triangle.
 * https://oeis.org/A289230
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6-6*n^5-24*n^4+220*n^3-153*n^2-1488*n+2592)/36 +
 *            IF(MOD(n,2)=1,-1)/2 + IF(MOD(n,3)=1,-2)/9
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 74, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The correction terms are integer divisions and evaluate to zero for all n. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* Compute polynomial using 128‑bit intermediate to avoid overflow */
    __int128 nn = n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;
    __int128 n6 = n5 * nn;

    __int128 numer = n6
                    - (__int128)6 * n5
                    - (__int128)24 * n4
                    + (__int128)220 * n3
                    - (__int128)153 * n2
                    - (__int128)1488 * nn
                    + (__int128)2592;

    seq_t result = (seq_t)(numer / 36);

    /* The following corrections are zero when using integer division. */
    if ((n & 1) == 1) { /* odd n */
        result -= (seq_t)((-1) / 2);
    }
    if (n % 3 == 1) {
        result -= (seq_t)((-2) / 9);
    }

    return result;
}
