/*
 * A297781  Number of distinct runs in base-13 digits of n.
 * https://oeis.org/A297781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct runs in the base‑13 digit expansion of
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the number of distinct runs of equal digits (digit,run_length) in the
 * base‑13 representation of n.  The result fits in seq_t because the maximum
 * number of distinct runs is bounded by 13 * number_of_digits(n) < 2^10. */
seq_t a(idx_t n)
{
    /* Handle the trivial case n==0 (outside the sequence range) safely */
    if (n == 0)
        return 0;

    /* Current digit and run length */
    uint8_t cur_digit = n % 13;
    idx_t    tmp      = n / 13;
    uint8_t  run_len  = 1;

    /* masks[d] holds bits representing run lengths that have already been seen
     * for digit d.  run_len ranges from 1 upwards; bit (run_len-1) is set when
     * that run length has been encountered.  Because n fits in uint64_t for
     * all provided test cases, the maximum run length is < 64. */
    uint64_t masks[13] = {0};
    uint64_t distinct  = 0;

    while (tmp) {
        uint8_t next_digit = tmp % 13;
        tmp /= 13;

        if (next_digit == cur_digit) {
            /* Continuing the current run */
            run_len++;
        } else {
            /* Finish current run */
            uint64_t bit = 1ULL << (run_len - 1);
            if (!(masks[cur_digit] & bit)) {
                masks[cur_digit] |= bit;
                distinct++;
            }
            /* Start new run */
            cur_digit = next_digit;
            run_len   = 1;
        }
    }

    /* Record the final run */
    uint64_t bit = 1ULL << (run_len - 1);
    if (!(masks[cur_digit] & bit)) {
        masks[cur_digit] |= bit;
        distinct++;
    }

    return (seq_t)distinct;
}
