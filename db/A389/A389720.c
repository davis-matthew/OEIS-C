/*
 * A389720  a(n) is the largest prime number obtainable by inserting a single
 *          digit anywhere in n (including at the beginning or end), or -1 if
 *          no such prime is obtainable.
 * https://oeis.org/A389720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = largest prime obtainable by inserting a single decimal
 *            digit anywhere in n (including at the start or end), or -1 if
 *            none
 * Range:     i8 n <= 10, i16 n <= 1000, i32 n <= 5247, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    /* Convert the index to a seq_t value */
    seq_t orig = (seq_t)n;

    /* The best prime found so far, -1 if none */
    seq_t best = (seq_t)-1;

    /* Number of decimal digits of the original number */
    int d = digit_len(orig);
    if (d < 0) return (seq_t)-1;  /* safety: should not happen */

    /* For each insertion position */
    for (idx_t pos = 0; pos <= (idx_t)d; ++pos) {
        /* 10^(d-pos) */
        seq_t pow_low = pow10(d - pos);
        seq_t high = orig / pow_low;
        seq_t low  = orig % pow_low;

        /* Try inserting each decimal digit */
        for (int dig = 0; dig <= 9; ++dig) {
            seq_t cand = high * pow_low * 10 + (seq_t)dig * pow_low + low;
            if (cand < 2) continue;             /* ignore numbers < 2 */
            if (is_prime_big(cand) && cand > best) {
                best = cand;
            }
        }
    }

    return best;
}
