/*
 * A269306  a(n+1) is the smallest integer such that the difference between
 *          its digital sum and the digital sum of a(n) is n.
 * https://oeis.org/A269306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0, a(n+1) is the smallest integer whose digital sum
 *            differs from that of a(n) by n. The sequence equals the smallest
 *            integer with digit sum n*(n-1)/2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"   /* assumed to provide seq_from_str */

/* Return the minimal integer with digit sum equal to T.  */
static seq_t minimal_with_digit_sum(seq_t T)
{
    /* digit sum = T = 9*q + r, 0 <= r < 9.  r is the leading digit
       (if r > 0).  The remaining q digits are all 9.  */
    seq_t q = T / 9;
    seq_t r = T % 9;

    /* # of decimal digits needed */
    size_t len = (size_t)q + (r ? 1 : 0);
    if (len == 0)
        return 0;               /* only possible when T == 0 (n == 1) */

    /* Allocate mutable buffer for the decimal representation. */
    char *s = (char *)malloc(len + 1);
    if (!s) abort();           /* fatal if allocation fails */

    size_t pos = 0;
    if (r) {
        /* r fits in 0..8, safe to cast to char. */
        s[pos++] = '0' + (int)r;
    }
    for (size_t i = 0; i < (size_t)q; ++i)
        s[pos++] = '9';
    s[pos] = '\0';

    seq_t result = seq_from_str(s);
    free(s);
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) = 0, for n > 1 compute a(n) = minimal int with digit sum
       (n-1)*n/2.  */
    if (n <= 1) return 0;

    seq_t nn = n;                               /* cast to seq_t */
    seq_t T = ((nn - 1) * nn) / 2;              /* triangular number */
    return minimal_with_digit_sum(T);
}
