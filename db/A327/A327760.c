/*
 * A327760  Primes in Rob Gahan's arithmetic progression of 27 primes.
 * https://oeis.org/A327760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = a1 + (n-1)*d
 * Range:     u8 none, u16 none, u32 none, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: full, pass, 27 terms through n = 27 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    static seq_t a1 = 0;
    static seq_t d  = 0;
    static bool init = false;
    if (!init) {
        a1 = seq_from_str("224584605939537911");
        d  = seq_from_str("18135696597948930");
        init = true;
    }
    seq_t m = (seq_t)(n - 1);
    return a1 + (m * d);
}
