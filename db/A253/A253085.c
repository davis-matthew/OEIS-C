/*
 * A253085  a(n) = A247649(A247647(n)).
 * https://oeis.org/A253085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A247649(A247647(n)), where A247647(n) is read as a binary
 *            number m (binary strings starting and ending in 1 with no "00"),
 *            and A247649(m) = number of odd coefficients of
 *            (1+x+x^2+x^3+x^4)^m, computed by square-and-multiply mod 2
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t a247647_binary_value(uint64_t n)
{
    uint64_t fib[92];
    int i;
    fib[0] = 0; fib[1] = 1;
    for (i = 2; i < 92; ++i) fib[i] = fib[i-1] + fib[i-2];

    uint64_t L = 1, cum = 0;
    for (;;) {
        if (cum + fib[L] >= n) break;
        cum += fib[L];
        ++L;
    }
    uint64_t k = n - cum;
    if (L == 1) return 1;
    uint64_t M = L - 2;
    uint64_t m = 1;
    int prev = 1;
    for (uint64_t pos = 0; pos < M; ++pos) {
        uint64_t rem = M - pos;
        uint64_t cnt0 = (prev == 1) ? fib[rem] : 0;
        if (cnt0 >= k) {
            m = m * 2;
            prev = 0;
        } else {
            k -= cnt0;
            m = m * 2 + 1;
            prev = 1;
        }
    }
    return m * 2 + 1;
}

static uint64_t a247649_count(uint64_t m)
{
    size_t cap = (size_t)(4 * m + 8);
    unsigned char *P = (unsigned char *)calloc(cap, 1);
    unsigned char *Q = (unsigned char *)calloc(cap, 1);
    if (!P || !Q) exit(EXIT_FAILURE);
    size_t d = 0;
    P[0] = 1;
    int top = 63;
    while (top >= 0 && !((m >> top) & 1)) --top;
    for (int b = top; b >= 0; --b) {
        /* square */
        size_t nd = d * 2;
        memset(Q, 0, nd + 1);
        for (size_t i = 0; i <= d; ++i)
            if (P[i]) Q[2 * i] = 1;
        { unsigned char *t = P; P = Q; Q = t; }
        d = nd;
        if ((m >> b) & 1) {
            memset(Q, 0, d + 5);
            for (size_t i = 0; i <= d; ++i)
                if (P[i]) {
                    Q[i] ^= 1; Q[i+1] ^= 1; Q[i+2] ^= 1; Q[i+3] ^= 1; Q[i+4] ^= 1;
                }
            { unsigned char *t = P; P = Q; Q = t; }
            d += 4;
        }
    }
    uint64_t cnt = 0;
    for (size_t i = 0; i <= d; ++i) cnt += P[i];
    free(P);
    free(Q);
    return cnt;
}

seq_t a(idx_t n)
{
    uint64_t m = a247647_binary_value((uint64_t)n);
    return (seq_t)a247649_count(m);
}
