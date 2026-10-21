/*
 * A289864  Number of cliques in the n-triangular honeycomb queen graph.
 * https://oeis.org/A289864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 - 18*n^2 - 68*n - 79 - (-1)^n + 3*2^(n + 5))/16
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 2, a(2) = 8, a(3) = 27, a(4) = 72, … */

#include <stdint.h>   /* for int64_t */

seq_t a(idx_t n)
{
    /* n >= 1 for correct use (the sequence is undefined for n = 0) */
    /*   3*2^(n+5) part */
    seq_t power = (seq_t)1;
    power <<= (unsigned long long)(n + 5);          /* 2^(n+5) */
    seq_t term  = (seq_t)3 * power;                /* 3*2^(n+5) */

    /* the polynomial part (64‑bit signed, enough for n up to at least 10000)   */
    int64_t nn  = (int64_t)n;
    int64_t poly = 4*nn*nn*nn - 18*nn*nn - 68*nn - 79
                  - ((nn & 1) == 0 ? 1 : -1);      /* -(-1)^n */

    if (poly >= 0) {
        term += (seq_t)poly;
    } else {
        term -= (seq_t)(-poly);
    }

    /* exact division by 16 (shift right 4 bits) */
    return term >> 4;
}
