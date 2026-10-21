/*
 * A298972  Number of positive integers k < n such that n occurs in the
 *          Reverse-and-Add trajectory of k.
 * https://oeis.org/A298972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) counts the integers k < n whose reverse‑and‑add trajectory
 *            contains n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x = x / 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t count = 0;
    seq_t nseq  = (seq_t)n;          /* local copy for comparison */
    for (idx_t k = 1; k < n; ++k) {
        seq_t x = (seq_t)k;
        while (x < nseq) {
            seq_t rev = reverse_digits(x);
            x += rev;
        }
        if (x == nseq)
            count += (seq_t)1;
    }
    return count;
}
