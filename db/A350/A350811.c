/*
 * A350811  a(n) = ceiling((n-R(n))^2/(n+R(n))), where R(n) is the digit
 *          reversal of n.
 * https://oeis.org/A350811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = ceil((n - R(n))^2/(n + R(n)))
 * Range:     u8 n <= 12, u16 n <= 224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the decimal digit reversal of n (as idx_t) and return it as seq_t. */
static seq_t rev_idx(idx_t x)
{
    seq_t rev = 0;
    while (x > 0)
    {
        rev = rev * 10 + (seq_t)(x % 10);
        x /= 10;
    }
    return rev;
}

/* Sequence A350811: a(n) = ceil((n - R(n))^2/(n + R(n))) where R(n) is the reverse of n. */
seq_t a(idx_t n)
{
    seq_t rev = rev_idx(n);
    seq_t n_ts = n;                /* cast idx_t to seq_t for arithmetic */

    /* Compute absolute difference |n - R(n)| */
    seq_t diff;
    if (n_ts >= rev)
        diff = n_ts - rev;
    else
        diff = rev - n_ts;

    /* If n is a palindrome, result is 0. */
    if (diff == 0)
        return 0;

    seq_t numer = diff * diff;      /* (n - R)^2 */
    seq_t denom = n_ts + rev;       /* n + R */

    /* Ceil of numer/denom: (numer + denom - 1) / denom */
    seq_t result = (numer + denom - 1) / denom;
    return result;
}
