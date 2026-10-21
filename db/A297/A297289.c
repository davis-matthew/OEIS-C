/*
 * A297289  Numbers whose base-16 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 4110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A297289: nth integer whose base-16 digits have equal down‑variation
   and up‑variation.
   Implementation: brute‑force search with incremental memoization. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static bool satisfies(seq_t x)
{
    /* Extract base‑16 digits of x (least significant first). */
    uint8_t digits[128];          /* enough for very large numbers */
    size_t dcnt = 0;
    seq_t t = x;
    while (t > 0) {
        digits[dcnt++] = (uint8_t)(t & 0xFULL);
        t = t >> 4;               /* divide by 16 */
    }
    if (dcnt == 0) {              /* x == 0, not used but keep safe */
        dcnt = 1;
        digits[0] = 0;
    }

    /* Compute down‑variation and up‑variation. */
    seq_t dv = 0, uv = 0;
    for (size_t i = dcnt - 1; i > 0; --i) {
        uint8_t dh = digits[i];
        uint8_t dl = digits[i - 1];
        if (dh > dl)
            dv += (seq_t)(dh - dl);
        else if (dh < dl)
            uv += (seq_t)(dl - dh);
    }
    return dv == uv;
}

seq_t a(idx_t n)
{
    /* static state holds the generated sequence */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static seq_t last_checked = 0;

    if (n == 0)
        return 0;                 /* undefined; harmless */

    /* Generate terms until we reach the requested index. */
    while (seq_len < (size_t)n) {
        last_checked = last_checked + 1;
        if (satisfies(last_checked)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                void *tmp = realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();    /* allocation failure */
                seq = (seq_t *)tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = last_checked;
        }
    }
    return seq[n - 1];
}
