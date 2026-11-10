/*
 * A335134  Fixed points of A335133.
 * https://oeis.org/A335134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th fixed point of A335133 (a(1)=0). Found by DFS over
 *            binary prefixes in increasing order: the left-diagonal bit k of
 *            the EQ-triangle depends only on the first k+1 bits, and a fixed
 *            point needs it to equal bit k
 * Range:     u8 n <= 60, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

#define A335134_MAXL 130

static unsigned char A335134_bits[A335134_MAXL];
static unsigned long long A335134_rem;

static int A335134_ok(int pos)
{
    unsigned char t[A335134_MAXL];
    int i, r;
    for (i = 0; i <= pos; ++i) t[i] = A335134_bits[i];
    for (r = 0; r < pos; ++r) {
        for (i = 0; i < pos - r; ++i) t[i] = (t[i] == t[i + 1]) ? 1 : 0;
    }
    return t[0] == A335134_bits[pos];
}

static int A335134_dfs(int pos, int L)
{
    int b;
    if (pos == L) {
        --A335134_rem;
        return A335134_rem == 0;
    }
    for (b = (pos == 0 ? 1 : 0); b <= 1; ++b) {
        A335134_bits[pos] = (unsigned char)b;
        if (A335134_ok(pos)) {
            if (A335134_dfs(pos + 1, L)) return 1;
        }
    }
    return 0;
}

seq_t a(idx_t n)
{
    int L, i;
    seq_t v = 0;
    if (n <= 1) return 0;
    A335134_rem = (unsigned long long)(n - 1);
    for (L = 1; L < A335134_MAXL; ++L) {
        if (A335134_dfs(0, L)) {
            for (i = 0; i < L; ++i) v = (v << 1) | (seq_t)A335134_bits[i];
            return v;
        }
    }
    return 0;
}
