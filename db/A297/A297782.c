/*
 * A297782  Number of distinct runs in base-14 digits of n.
 * https://oeis.org/A297782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number of distinct runs in the base‑14 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 gracefully (although the sequence starts at 1). */
    if (n == 0)
        return 0;

    /* Extract base‑14 digits (least‑significant first). */
    uint8_t d[64];
    size_t cnt = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        d[cnt++] = (uint8_t)(tmp % 14);
        tmp /= 14;
    }

    /* Store distinct runs as (digit, length). */
    struct Run { uint8_t digit; uint8_t len; };
    struct Run seen[64];
    size_t seen_cnt = 0;
    seq_t distinct = 0;

    if (cnt == 0)
        return 0;   /* should not happen for n > 0 */

    uint8_t prev = d[0];
    uint8_t runlen = 1;

    for (size_t i = 1; i < cnt; ++i) {
        uint8_t curr = d[i];
        if (curr == prev) {
            ++runlen;
        } else {
            /* Record the finished run if it is new. */
            int found = 0;
            for (size_t k = 0; k < seen_cnt; ++k) {
                if (seen[k].digit == prev && seen[k].len == runlen) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                seen[seen_cnt].digit = prev;
                seen[seen_cnt].len   = runlen;
                ++seen_cnt;
                ++distinct;
            }
            prev = curr;
            runlen = 1;
        }
    }

    /* Final run. */
    int found = 0;
    for (size_t k = 0; k < seen_cnt; ++k) {
        if (seen[k].digit == prev && seen[k].len == runlen) {
            found = 1;
            break;
        }
    }
    if (!found) {
        seen[seen_cnt].digit = prev;
        seen[seen_cnt].len   = runlen;
        ++seen_cnt;
        ++distinct;
    }

    return distinct;
}
