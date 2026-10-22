/*
 * A380389  Array read by ascending antidiagonals: A(n, k) is equal to n/k if
 *          k | n, else to the concatenation of the numerator and the
 *          denominator of n/k.
 * https://oeis.org/A380389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look, tabl
 * Formula:   a(n) is the entry read from the infinite array A(i,j) defined by
 *            A(i,j) = i/j if j divides i A(i,j) = concat( reduced i/j )
 *            otherwise, where the array is read by ascending antidiagonals
 *            with increasing i+j and, for each diagonal, by increasing j
 * Range:     u8 n <= 75, u16 n <= 9737, u32 n <= 9737, u64 n <= 9737,
 *            u128 n <= 9737, big any n
 * Helpers:   digit_len, gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* minimal helper headers from the utils library */
#include "../utils/gcd.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* external linkage as required by the build system */
seq_t a(idx_t n)
{
    /* n >= 1, find diagonal s such that (s-1)*s/2 >= n */
    idx_t s = 2;                     /* smallest diagonal with at least one entry */
    while ((s - 1) * s / 2 < n)      /* linear scan is sufficient for n <= 10^4 */
        ++s;

    idx_t prev_total = (s - 2) * (s - 1) / 2;
    idx_t offset = n - prev_total;   /* 1 .. (s-1) */
    idx_t j = offset;                /* column index */
    idx_t i = s - j;                 /* row index */

    /* compute the value of A(i,j) */
    if ((i % j) == 0)                /* integer division */
        return (seq_t)(i / j);

    /* reduce the fraction */
    seq_t num = (seq_t)i;
    seq_t den = (seq_t)j;
    seq_t g = gcd(num, den);
    num /= g;
    den /= g;

    /* concatenate: num * 10^{digits(den)} + den */
    seq_t pow = pow10(digit_len(den));
    return num * pow + den;
}
