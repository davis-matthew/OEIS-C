/*
 * A400469  Deterministic version of the random Fibonacci sequence, with signs
 *          from A076478 and the seed as (1,0).
 * https://oeis.org/A400469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) + a(n-2) if A076478(n-2) = 1, else a(n-1) -
 *            a(n-2); a(0) = 1, a(1) = 0
 * Range:     i8 n <= 61, i16 n <= 92, i32 n <= 188, i64 n <= 430,
 *            i128 n <= 810, big any n
 * Validated: standard, pass, 83 terms through n = 9275 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A400469_dummy_unused;

seq_t a(idx_t n)
{
    seq_t p = 1;  /* a(k-2) */
    seq_t q = 0;  /* a(k-1) */
    if (n == 0) return p;
    if (n == 1) return q;

    idx_t m = 2;        /* current integer whose bits (minus leading 1) are emitted */
    unsigned len = 1;   /* number of bits emitted by m */
    unsigned j = 0;     /* next bit position within m */

    for (idx_t k = 2; k <= n; ++k) {
        unsigned bit = (unsigned)((m >> (len - 1 - j)) & 1U);
        seq_t r = bit ? (q + p) : (q - p);
        p = q;
        q = r;
        ++j;
        if (j == len) {
            j = 0;
            ++m;
            unsigned l = 0;
            idx_t t = m;
            while (t != 0) { t >>= 1; ++l; }
            len = l - 1;
        }
    }
    return q;
}
