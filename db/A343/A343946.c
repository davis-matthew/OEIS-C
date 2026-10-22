/*
 * A343946  Surface area of a right prism whose base is a regular n-gon with
 *          unit side length and unit height (rounded to the nearest integer).
 * https://oeis.org/A343946
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n + n*cot(π/n)/2)
 * Range:     u8 n <= 37, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Computes the surface area of a right prism whose base is a regular n‑gon
 * with unit side length and unit height, rounded to the nearest integer.
 * The function respects the OEIS offset: a(3) is the first term.
 */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;  /* the sequence is defined for n ≥ 3 */

    /* high‑precision constant for π */
    static const long double PI = 3.141592653589793238462643383279502884L;

    long double angle  = PI / (long double)n;          /* π/n  */
    long double sine   = sinl(angle);                  /* sin(π/n)        */
    long double cosine = cosl(angle);                  /* cos(π/n)        */

    /* term = n * cot(π/n) / 2  = n * cos(π/n) / (2*sin(π/n)) */
    long double term = (long double)n * cosine / (sine * 2.0L);

    /* total surface area before rounding */
    long double area = (long double)n + term;

    /* round to nearest integer and cast to seq_t */
    return (seq_t)llroundl(area);
}
