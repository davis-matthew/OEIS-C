/*
 * A363993  a(n) = greatest integer k such that 1/n + 1/(n + 1) +...+ 1/k < 2.
 * https://oeis.org/A363993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest integer k such that Σ_{i=n}^{k} 1/i < 2 (i.e. the
 *            harmonic series tail from n to k stays below 2)
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a double accumulator for the partial sum.
       The error in a double sum after ~70000 terms is far below
       the minimum increment 1/k (~1e-5), so the comparison to 2
       can be performed safely. */
    double sum = 0.0;
    idx_t k = n;
    while (1) {
        sum += 1.0 / (double)k;
        if (sum >= 2.0) {
            break;           /* the sum just exceeded 2 */
        }
        ++k;
    }
    return (seq_t)(k - 1);
}
