/*
 * A336781  a(n) is the least number of repetitions such that the result of
 *          the repeated execution of the division f <- f/n started at f=1
 *          produces 0, when the division is performed using 64-bit double
 *          precision floats according to the IEEE 754 standard.
 * https://oeis.org/A336781
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = number of iterations of the double‑precision division f ←
 *            f / n (starting from f = 1.0) until f becomes 0.0, plus one
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* value irrelevant below the offset 2 */
    double f  = 1.0;
    double dn = (double)n;        /* convert once to avoid repeated casts */
    seq_t k   = 0;
    while (f != 0.0) {
        f /= dn;
        ++k;
    }
    return k + 1;                  /* match OEIS definition */
}
