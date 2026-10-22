/*
 * A376313  Independence number of the 2-supertoken graph FF_2(C_n) of the
 *          cycle C_n on n vertices.
 * https://oeis.org/A376313
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%4==0 || n%4==1) ? (n/4)*(n+2) : ((n/4)+1)*n
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;        /* undefined for n < 2, return 0 */
    idx_t k = n / 4;
    seq_t sn = (seq_t)n;
    switch (n & 3) {
        case 0:
        case 1:
            return (seq_t)k * (sn + 2);
        case 2:
        case 3:
            return ((seq_t)(k + 1)) * sn;
    }
    return 0; /* unreachable */
}
