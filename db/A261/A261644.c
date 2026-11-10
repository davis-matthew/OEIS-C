/*
 * A261644  Distance of A260273(n) to next power of 2.
 * https://oeis.org/A261644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A062383(A260273(n)) - A260273(n); A260273(1)=1,
 *            A260273(k+1) = A260273(k) + (smallest positive integer whose
 *            binary form is not a substring of A260273(k)'s binary form).
 *            Computed by direct iteration
 * Range:     u8 n <= 60, u16 n <= 17456, u32 n <= 126224, u64 n <= 126224,
 *            u128 n <= 126224, big any n
 * Validated: standard, pass, 81 terms through n = 8267 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stddef.h>

static unsigned A261644_bitlen(seq_t v)
{
    unsigned len = 0;
    seq_t tmp = v;
    while (tmp != (seq_t)0) {
        ++len;
        tmp >>= 1;
    }
    return len;
}

static void A261644_bits(seq_t v, char *buf, size_t bufsize)
{
    unsigned len = A261644_bitlen(v);
    if (len == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    if (len >= bufsize) len = (unsigned)(bufsize - 1);
    unsigned i;
    for (i = 0; i < len; ++i)
        buf[len - 1 - i] = ((v >> i) & (seq_t)1) ? '1' : '0';
    buf[len] = '\0';
}

static seq_t A261644_missing(seq_t x)
{
    static char bitsx[8192];
    static char bitsp[512];

    A261644_bits(x, bitsx, sizeof(bitsx));

    seq_t k = (seq_t)1;
    while (true) {
        A261644_bits(k, bitsp, sizeof(bitsp));
        if (strstr(bitsx, bitsp) == NULL)
            return k;
        ++k;
    }
}

seq_t a(idx_t n)
{
    static bool have = false;
    static idx_t cn = 1;
    static seq_t cv = 1;

    if (n == 0)
        return (seq_t)0;

    if (!have || n < cn) {
        cn = 1;
        cv = (seq_t)1;
        have = true;
    }
    while (cn < n) {
        seq_t add = A261644_missing(cv);
        cv = cv + add;
        ++cn;
    }

    seq_t v = cv;
    seq_t p = (seq_t)1;
    while (p <= v) {
        p <<= 1;
        if (p == (seq_t)0) break;
    }
    return (seq_t)(p - v);
}
