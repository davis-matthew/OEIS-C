/*
 * A242063  Analog clock times where the minute hand is on the hour hand (in
 *          hhmm format).
 * https://oeis.org/A242063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   Let h = (n-1) + (n>11?1:0) // hour (0,1,…,10,12,13,…) Let k = h
 *            % 12 Let m = (60*k + 5)/11 // round 60*k/11 to nearest minute
 *            a(n) = h*100 + m
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t h = (seq_t)(n - 1);
    if (n > 11) h += 1;                // shift after 10:55
    seq_t k = h % 12;
    seq_t m = (60 * k + 5) / 11;
    return h * 100 + m;
}
