/*
 * A279934  Positive integers k such that {(k-1)*r} > 1/2, where r = golden
 *          ratio = (1 + sqrt(5))/2 and { } = fractional part.
 * https://oeis.org/A279934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer k for which the fractional part
 *            of (k-1)·φ is > ½, where φ = (1+√5)/2. The condition is
 *            equivalent to (k-1)+⌊(k-1)·√5⌋ being odd. The routine below
 *            iterates m = k-1, counts the occurrences where this parity is
 *            odd, and returns the corresponding k (= m+1). It uses...
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 754, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root: floor( sqrt(x) ) */
static seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) >> 1;    /* midpoint rounded up */
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t m = 0;          /* m = k - 1 */
    idx_t count = 0;      /* number of qualifying m seen so far */

    for (;;) {
        seq_t mm  = (seq_t)m;
        seq_t val = 5 * mm * mm;          /* 5·m²   */
        seq_t s   = isqrt_seq(val);       /* ⌊m√5⌋  */

        if (((mm + s) & 1) != 0) {        /* parity test */
            count++;
            if (count == n)
                return mm + 1;            /* k = m + 1 */
        }
        m++;
    }
}
