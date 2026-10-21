/*
 * A296160  Sum of the larger parts of the partitions of n into two parts such
 *          that the smaller part is even.
 * https://oeis.org/A296160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n-i) * ((i+1)%2) * = t*n -
 *            t*(t+1), where t = floor(floor(n/2)/2)
 * Range:     u8 n <= 38, u16 n <= 591, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* t = floor(floor(n/2)/2) */
    idx_t t = (n / 2) / 2;
    seq_t tn    = (seq_t) t * (seq_t) n;
    seq_t ttp1  = (seq_t) t * (seq_t)(t + 1);
    return tn - ttp1;
}
