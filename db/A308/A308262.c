/*
 * A308262  Numbers m such that A048385(m) ends with m.
 * https://oeis.org/A308262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number m >= 0 such that the decimal string of
 *            A048385(m) ends with the decimal string of m (found by
 *            sequential search, memoized)
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A308262_check(uint64_t m)
{
    char d[24];   /* digits of m, least significant first */
    char s[48];   /* decimal digits of A048385(m), most significant first */
    int L = 0, len = 0, i;
    uint64_t t = m;

    if (m == 0) return 1;
    while (t > 0) {
        d[L++] = (char)(t % 10);
        t /= 10;
    }
    for (i = L - 1; i >= 0; --i) {
        int q = d[i] * d[i];
        if (q >= 10) s[len++] = (char)(q / 10);
        s[len++] = (char)(q % 10);
    }
    for (i = 0; i < L; ++i) {
        if (s[len - 1 - i] != d[i]) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t terms[200000];
    static uint64_t count = 0;
    static uint64_t next = 0;

    if (n == 0) return 0;
    while (count < (uint64_t)n && count < 200000) {
        if (A308262_check(next)) terms[count++] = next;
        next++;
    }
    return (seq_t)terms[n - 1];
}
