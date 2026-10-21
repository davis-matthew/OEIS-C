/*
 * A280989  Rounded percent probability that a positive test result for some
 *          condition is a true positive when n% of the tested population has
 *          that condition and the test is 95% accurate.
 * https://oeis.org/A280989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = round(950*n/(9*n + 50)) for n >= 0
 * Range:     u8 n <= 0, u16 n <= 68, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the rounded percentage of true positives for a 95% accurate test. */
seq_t a(idx_t n)
{
    seq_t num = (seq_t)950 * n;          /* numerator 950·n */
    seq_t den = (seq_t)9 * n + (seq_t)50;/* denominator 9·n + 50 */
    /* Round to nearest integer: (num + den/2) / den  (den > 0) */
    return (num + (den >> 1)) / den;
}
