/*
 * A302766  a(n) = n*((4*n + 1)*(7*n - 4) + 15*n*(-1)^n)/48.
 * https://oeis.org/A302766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A302766 – 6‑dimensional convolutions
 *
 *  Formula:
 *      a(n) = n*((4*n + 1)*(7*n - 4) + 15*n*(-1)^n)/48
 *
 *  Equivalent closed forms:
 *      a(n) = (14*n³ + 3*n² - 2*n)/24   for even n
 *      a(n) = (14*n³ - 12*n² - 2*n)/24  for odd  n
 *
 *  The implementation uses only integer arithmetic and avoids
 *  overflow for all n for which the result fits in the target
 *  unsigned integer type seq_t.  A safe intermediate division
 *  by 24 keeps the products within 64‑bit bounds when seq_t
 *  is 64 bits (the sequence grows as O(n³)), allowing fast
 *  evaluation up to hundreds of thousands of terms.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    if (n < 1)            /* offset is 1 – a(0) is undefined */
        return 0;

    seq_t nt  = (seq_t)n;     /* promote index to seq_t */
    seq_t n2  = nt * nt;      /* n² – fits in seq_t for all relevant n */

    seq_t inside;
    /* Compute the inner expression depending on the parity of n */
    if ((n & 1u) == 0u) {     /* even n */
        inside = 14 * n2 + 3 * nt - 2;
    } else {                 /* odd n */
        inside = 14 * n2 - 12 * nt - 2;
    }

    /* Split the division by 24 to keep intermediates small */
    seq_t part  = inside / 24u;
    seq_t res   = nt * part;          /* main part of the quotient */
    seq_t rem   = inside % 24u;
    res += (nt * rem) / 24u;          /* remainder part */

    return res;
}
