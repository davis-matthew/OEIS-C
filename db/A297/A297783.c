/*
 * A297783  Number of distinct runs in base-15 digits of n.
 * https://oeis.org/A297783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number of distinct runs in the base‑15 digits of n (a(n) =
 *            number of distinct run lengths with their digits)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 0 is not used in the OEIS table (offset 1), but return 0 safely */
    if (n == 0) return 0;

    /* Store base‑15 digits, least significant first (max 128 digits is enough) */
    unsigned digits[128];
    size_t len = 0;
    idx_t t = n;
    while (t > 0) {
        digits[len++] = (unsigned)(t % 15);
        t /= 15;
    }

    /* Gather runs: each run has a digit and a length */
    unsigned run_digit[128];
    unsigned run_len[128];
    size_t run_cnt = 0;

    unsigned cur_digit = digits[0];
    unsigned cur_len = 1;
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == cur_digit) {
            ++cur_len;
        } else {
            run_digit[run_cnt] = cur_digit;
            run_len[run_cnt]   = cur_len;
            ++run_cnt;
            cur_digit = digits[i];
            cur_len   = 1;
        }
    }
    /* last run */
    run_digit[run_cnt] = cur_digit;
    run_len[run_cnt]   = cur_len;
    ++run_cnt;

    /* Count distinct run types (digit and length together) */
    seq_t distinct = 0;
    for (size_t i = 0; i < run_cnt; ++i) {
        bool seen = false;
        for (size_t j = 0; j < i; ++j) {
            if (run_digit[i] == run_digit[j] && run_len[i] == run_len[j]) {
                seen = true;
                break;
            }
        }
        if (!seen) ++distinct;
    }

    return distinct;
}
