/*
 * A338952  Number of oriented colorings of the 96 edges (or triangular faces)
 *          of the 4-D 24-cell using subsets of a set of n colors.
 * https://oeis.org/A338952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (96*n^8 + 144*n^12 + 48*n^16 + 32*n^18 + 84*n^24 +
 *            48*n^32 + 32*n^36 + 19*n^48 + 72*n^50 + n^96) / 576
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;

    /* Powers up to n^96. 96 multiplications per call. */
    seq_t pow[97];
    pow[0] = (seq_t)1;
    for (idx_t i = 1; i <= 96; ++i) {
        pow[i] = pow[i-1] * n_seq;
    }

    seq_t sum = (seq_t)0;
    sum += (seq_t)96  * pow[ 8];
    sum += (seq_t)144 * pow[12];
    sum += (seq_t)48  * pow[16];
    sum += (seq_t)32  * pow[18];
    sum += (seq_t)84  * pow[24];
    sum += (seq_t)48  * pow[32];
    sum += (seq_t)32  * pow[36];
    sum += (seq_t)19  * pow[48];
    sum += (seq_t)72  * pow[50];
    sum += (seq_t)1   * pow[ 96];

    return sum / (seq_t)576;
}
