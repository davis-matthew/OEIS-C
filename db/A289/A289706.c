/*
 * A289706  Number of 5-cycles in the n-triangular honeycomb queen graph.
 * https://oeis.org/A289706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1095 - 4770*n + 934*n^2 + 4680*n^3 - 3170*n^4 + 540*n^5
 *            + 16*n^6 - 15*(-1)^n*(73 - 30*n + 2*n^2))/320
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 62, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute with 128‑bit signed intermediates to avoid overflow. */
    __int128 N  = n;
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n3 * N;
    __int128 n5 = n4 * N;
    __int128 n6 = n5 * N;

    __int128 num = 1095 - 4770 * N
                    + 934 * n2
                    + 4680 * n3
                    - 3170 * n4
                    + 540 * n5
                    + 16 * n6;

    /* Parity adjustment. */
    __int128 parity_inner = 73 - 30 * N + 2 * n2;
    if ((n & 1) == 0)   /* even n */
        num -= 15 * parity_inner;
    else                 /* odd n */
        num += 15 * parity_inner;

    __int128 val = num / 320;   /* exact division. */
    return (seq_t)val;
}
