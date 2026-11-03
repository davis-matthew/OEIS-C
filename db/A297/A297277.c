/*
 * A297277  Numbers whose base-12 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 5472, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9098 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑12 digits have equal down‑variation and up‑variation.
   Computed by scanning natural numbers and keeping those that satisfy
   dv = uv. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Test whether a number has equal down‑variation and up‑variation in base 12. */
static bool _good_base12(seq_t x)
{
    /* Base‑12 digits least significant first. */
    uint8_t d[64];
    size_t len = 0;
    seq_t t = x;
    while (t != 0) {
        d[len++] = (uint8_t)(t % 12);
        t /= 12;
    }

    /* Single‑digit numbers satisfy the condition. */
    if (len <= 1) return true;

    seq_t dv = 0, uv = 0;
    /* Process digits from most to least significant. */
    for (size_t i = len - 1; i > 0; --i) {
        int diff = (int)d[i] - (int)d[i - 1];
        if (diff > 0) {
            dv += (seq_t)diff;
        } else if (diff < 0) {
            uv += (seq_t)(-diff);
        }
    }
    return dv == uv;
}

/* Return the nth term of A297277 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */
    idx_t count = 0;
    seq_t num = 0;
    while (true) {
        ++num;                     /* start at 1 */
        if (_good_base12(num)) {
            ++count;
            if (count == n) return num;
        }
    }
}
