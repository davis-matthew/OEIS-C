/*
 * A320485  Keep just the digits of n that appear exactly once; write -1 if
 *          all digits disappear.
 * https://oeis.org/A320485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   not stated
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A320485: for a given non‑negative integer n,
   keep only the digits that occur exactly once in its decimal
   representation, remove any leading zeros, and return -1 if
   no digits remain. */
seq_t a(idx_t n)
{
    /* Special case: n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* Count occurrences of each digit and collect digits in reverse order. */
    unsigned char cnt[10] = {0};
    unsigned char rev[20];          /* enough for an uint64_t (max 20 decimal digits) */
    unsigned rev_len = 0;

    idx_t tmp = n;
    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        tmp /= 10;
        rev[rev_len++] = (unsigned char)d;
        cnt[d]++;
    }

    /* Keep only digits that appeared exactly once, preserving original order. */
    unsigned char keep[20];
    unsigned keep_len = 0;
    for (unsigned i = 0; i < rev_len; i++) {
        unsigned d = rev[rev_len - 1 - i];
        if (cnt[d] == 1)
            keep[keep_len++] = (unsigned char)d;
    }

    /* If no digit remains, return -1. */
    if (keep_len == 0)
        return (seq_t)-1;

    /* Skip leading zeros unless all remaining digits are zeros. */
    unsigned first_nonzero = 0;
    while (first_nonzero < keep_len && keep[first_nonzero] == 0)
        first_nonzero++;

    if (first_nonzero == keep_len)
        return (seq_t)0;

    /* Build the resulting number from the first non‑zero digit onward. */
    seq_t result = 0;
    for (unsigned i = first_nonzero; i < keep_len; i++)
        result = result * 10 + (seq_t)keep[i];

    return result;
}
