/*
 * A269287  Number of nX6 0..3 arrays with some element plus some horizontally
 *          or antidiagonally adjacent neighbor totalling three no more than
 *          once.
 * https://oeis.org/A269287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 564*a(n-1) -87462*a(n-2) +2257724*a(n-3) -21169617*a(n-4)
 *            +76236552*a(n-5) -92236816*a(n-6) for n>7 Initial values (offset
 *            1): a(1) = 2592 a(2) = 1363572 a(3) = 544118148 a(4) =
 *            193446959076 a(5) = 64538382287988 a(6) = 20680187784811380 a(7)
 *            = 6444304707617709156
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == 0) return 0;                 /* unused but harmless */

    /* Pre‑computed first seven terms */
    static const seq_t first[8] = {
        0ULL,                              /* dummy for index 0 */
        2592ULL,
        1363572ULL,
        544118148ULL,
        193446959076ULL,
        64538382287988ULL,
        20680187784811380ULL,
        6444304707617709156ULL
    };

    if (n <= 7)  return first[n];

    /* We maintain a sliding window of the last six terms
       a(n-6), a(n-5), a(n-4), a(n-3), a(n-2), a(n-1)   */
    seq_t prev[6];
    prev[0] = first[2];   /* a(n-6)   for n=8 */
    prev[1] = first[3];   /* a(n-5)   */
    prev[2] = first[4];   /* a(n-4)   */
    prev[3] = first[5];   /* a(n-3)   */
    prev[4] = first[6];   /* a(n-2)   */
    prev[5] = first[7];   /* a(n-1)   */

    /* Compute terms from 8 up to n */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t sumPos = prev[5] * ((seq_t)564)   /* a(n-1)  */
                      + prev[3] * ((seq_t)2257724)/* a(n-3)  */
                      + prev[1] * ((seq_t)76236552);/* a(n-5)*/

        seq_t sumNeg = prev[4] * ((seq_t)87462)  /* a(n-2)  */
                      + prev[2] * ((seq_t)21169617)/* a(n-4)  */
                      + prev[0] * ((seq_t)92236816);/* a(n-6)*/

        seq_t cur = sumPos - sumNeg;           /* a(n)   */

        /* Shift window for next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = cur;
    }

    return prev[5];   /* this now equals a(n)   */
}
