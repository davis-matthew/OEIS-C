/*
 * A261018  First differences of A260273.
 * https://oeis.org/A261018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A261461(A260273(n)), where A260273(1)=1, A260273(m+1) =
 *            A260273(m) + A261461(A260273(m)); A261461(x) = smallest positive
 *            integer whose binary form is not a substring of x's (direct
 *            simulation)
 * Range:     u8 n <= 63, u16 n <= 9877, u32 n <= 19999, u64 n <= 19999,
 *            u128 n <= 19999, big any n
 * Validated: standard, pass, 105 terms through n = 9888 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stddef.h>

static size_t A261018_tobits(seq_t v, char *buf)
{
    char tmp[8192];
    size_t len = 0;
    while (v != 0 && len < sizeof(tmp) - 1) {
        tmp[len++] = (char)('0' + (int)(v & 1));
        v >>= 1;
    }
    for (size_t i = 0; i < len; ++i) buf[i] = tmp[len - 1 - i];
    buf[len] = '\0';
    return len;
}

static seq_t A261018_missing(seq_t x)
{
    static char bx[8200];
    static char bp[8200];
    A261018_tobits(x, bx);
    seq_t k = 1;
    while (true) {
        A261018_tobits(k, bp);
        if (strstr(bx, bp) == NULL) return k;
        ++k;
    }
}

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t cx;
    static idx_t ci = 0;
    if (!init || n < ci) {
        cx = 1;
        ci = 1;
        init = true;
    }
    while (ci < n) {
        cx = cx + A261018_missing(cx);
        ++ci;
    }
    return A261018_missing(cx);
}
