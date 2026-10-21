/*
 * A256436  Characteristic function of pentatope numbers.
 * https://oeis.org/A256436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a pentatope number, otherwise 0. Pentatope
 *            numbers are of the form n = k*(k+1)*(k+2)*(k+3)/24. The function
 *            uses integer arithmetic only
 * Range:     u8 n <= 9863, u16 n <= 9863, u32 n <= 9863, u64 n <= 9863,
 *            u128 n <= 9863, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    /* Compute m = floor((24*n)^(1/4)). */
    unsigned __int128 sq = (unsigned __int128)24 * (unsigned __int128)n;
    uint64_t m = 0;
    for (;;) {
        uint64_t cand = m + 1;
        unsigned __int128 cand4 = (unsigned __int128)cand
                                 * cand
                                 * cand
                                 * cand;
        if (cand4 > sq) break;
        m = cand;
    }
    /* Compute binomial(m+2,4) = (m+2)*(m+1)*m*(m-1)/24. */
    uint64_t k = m + 2;
    unsigned __int128 prod = (unsigned __int128)k * (k - 1) * (k - 2) * (k - 3);
    prod /= 24;
    return (seq_t)(prod == (unsigned __int128)n ? 1 : 0);
}
