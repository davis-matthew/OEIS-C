/*
 * A362712  Number of n-colorings of the Goldner-Harary graph.
 * https://oeis.org/A362712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^11 -27*n^10 +326*n^9 -2316*n^8 +10710*n^7 * -33642*n^6
 *            +72576*n^5 -105948*n^4 +99873*n^3 * -54675*n^2 +13122*n
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 10, u64 n <= 58, u128 n <= 3185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t) n;
    seq_t p[12];              /* p[i] = n^i   (i from 0 to 11) */
    p[1] = nn;
    for (int i = 2; i <= 11; ++i)
        p[i] = p[i-1] * nn;

    seq_t sum_pos = p[11];
    sum_pos += p[9] * (seq_t)326;
    sum_pos += p[7] * (seq_t)10710;
    sum_pos += p[5] * (seq_t)72576;
    sum_pos += p[3] * (seq_t)99873;
    sum_pos += p[1] * (seq_t)13122;

    seq_t sum_neg = p[10] * (seq_t)27;
    sum_neg += p[8] * (seq_t)2316;
    sum_neg += p[6] * (seq_t)33642;
    sum_neg += p[4] * (seq_t)105948;
    sum_neg += p[2] * (seq_t)54675;

    return sum_pos - sum_neg;
}
