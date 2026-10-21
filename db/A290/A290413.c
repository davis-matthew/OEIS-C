/*
 * A290413  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 785", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=101, a(3)=111, a(4)=10011, a(5)=111111. For
 *            n≥6 a(n) is the decimal number consisting of k ones, where k=
 *            (n/2) if n is even, and (n+1) if n is odd. Thus a(6)=111,
 *            a(7)=11111111, a(8)=1111, a(9)=1111111111, …
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t parse_decimal(const char *s)
{
    seq_t v = 0;
    for ( ; *s ; ++s )
        v = v * (seq_t)10 + (seq_t)(*s - '0');
    return v;
}

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return parse_decimal("1");
        case 1: return parse_decimal("1");
        case 2: return parse_decimal("101");
        case 3: return parse_decimal("111");
        case 4: return parse_decimal("10011");
        case 5: return parse_decimal("111111");
    }
    idx_t k = ((n & 1) ? (n + 1) : (n >> 1));
    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i)
        val = val * (seq_t)10 + (seq_t)1;
    return val;
}
