/*
 * A386905  Distinct terms in A385988.
 * https://oeis.org/A386905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th distinct term of A385988 (1, 2, 3, 7, 23, 11415,
 *            ...). Computed by jumping: from K with sum 2^m, the next K' = K
 *            + (2^M - 2^m)/K where M > m is the smallest exponent with K |
 *            2^M - 2^m
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static idx_t count = 0;
    static idx_t cap = 0;
    static long long m = 0;   /* current partial sum is 2^m at index terms[count] */

    if (n == 0) return (seq_t)0;

    if (n + 1 > cap) {
        idx_t nc = (cap == 0) ? 16 : cap * 2;
        if (nc < n + 1) nc = n + 1;
        terms = mem_resize(seq_t, terms, nc);
        cap = nc;
    }
    if (count == 0) {
        terms[1] = (seq_t)1;
        count = 1;
        m = 0;
    }
    while (count < n) {
        seq_t K = terms[count];
        seq_t r0 = (seq_t)1 % K;
        for (long long i = 0; i < m; ++i) r0 = (r0 * 2) % K;
        seq_t r = r0;
        long long d = 0;
        for (long long i = 1; i <= 10000000LL; ++i) {
            r = (r * 2) % K;
            if (r == r0) { d = i; break; }
        }
        if (d == 0) return (seq_t)0;
        long long M = m + d;
        seq_t pw = 1;
        for (long long i = 0; i < M; ++i) pw = pw * 2;
        seq_t low = (seq_t)1;
        for (long long i = 0; i < m; ++i) low = low * 2;
        seq_t step = (pw - low) / K;
        terms[count + 1] = K + step;
        m = M;
        ++count;
    }
    return terms[n];
}
