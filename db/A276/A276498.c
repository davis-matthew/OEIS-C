/*
 * A276498  Number of trials T of an event that occurs with probability 1-1/n
 *          that must be repeated consecutively so that the probability that
 *          the event occurs on all T trials (i.e., (1-1/n)^T) is minimally
 *          higher than 1/n.
 * https://oeis.org/A276498
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(log(1/n)/log(1-1/n))
 * Range:     u8 n <= 62, u16 n <= 7360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* a(n) is defined only for n >= 2 */

    /* Compute the ratio using double precision and round to the nearest integer. */
    double inv = 1.0 / (double)n;          /* 1/n          */
    double num = log(inv);                 /* log(1/n)     */
    double den = log(1.0 - inv);           /* log(1-1/n)   */
    double val = num / den;

    uint64_t r = (uint64_t)(val + 0.5);     /* round to integer */
    return (seq_t)r;
}
