/*
 * A333442  For any n > 0, let Sum_{k >= 0} d_k / 10^k be the decimal
 *          representation of 1/n; a(n) is the least m such that d_m = max_{k
 *          >= 0} d_k.
 * https://oeis.org/A333442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the index of the first occurrence of the maximal * digit
 *            in the decimal representation of 1/n. * Computed by a single
 *            pass of long‑division, stopping either when * the digit 9 is
 *            reached or when a remainder repeats
 * Range:     u8 n <= 1386, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1.  The integer part is 1, so
     * d0 = 1 is the maximal digit occurring at position 0. */
    if (n == 1)
        return 0;

    /* Cast n to seq_t for arithmetic that may involve larger values. */
    const seq_t n_seq = (seq_t)n;

    /* maximum digit seen so far; initial maximum equals the integer
     * part (0 for n > 1). */
    seq_t maxd = 0;
    /* position (index) of the first occurrence of that maximum digit. */
    seq_t firstpos = 0;

    /* position counter: d0 corresponds to position 0,
     * the first fractional digit will get position 1. */
    idx_t pos = 1;

    /* remainder after removing the integer part ; for n>1 this is 1. */
    seq_t r = 1 % n_seq;

    /* Array to remember which remainders have already been seen.
     * Size n is sufficient because remainders are in [0, n−1]. */
    unsigned char *seen = (unsigned char *)calloc((size_t)n, sizeof(unsigned char));
    if (!seen)
        return 0;   /* Memory allocation failed – return a safe value. */

    /* Simulate long division until a remainder repeats or the digit 9 is found. */
    while (!seen[(idx_t)r]) {
        seen[(idx_t)r] = 1;

        /* generate the next decimal digit */
        seq_t digit = (r * (seq_t)10) / n_seq;
        r = (r * (seq_t)10) % n_seq;

        /* update maximum digit information */
        if (digit > maxd) {
            maxd = digit;
            firstpos = pos;
            if (digit == 9)        /* cannot get a larger digit */
                break;
        }
        ++pos;
    }

    free(seen);
    return (seq_t)firstpos;
}
