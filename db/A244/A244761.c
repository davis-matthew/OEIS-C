/*
 * A244761  Numbers obtained by concatenating the squares of the digits of
 *          Lucas(n).
 * https://oeis.org/A244761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the squares of the digits of Lucas(n)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   digit_len, pow10, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/seq_to_str.h"
#include "../utils/pow10.h"

/* fast doubling Fibonacci: returns F(n) in *fn and F(n+1) in *fn1 */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1) */
    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);
    seq_t d = a * a + b * b;
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Lucas number L(n): 2,1,3,4,7,... */
static seq_t lucas_n(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    seq_t f_k, f_k1;
    fib_pair(n - 1, &f_k, &f_k1);        /* f_k  = F(n-1), f_k1 = F(n) */
    return f_k1 + (seq_t)2 * f_k;        /* L(n) = F(n) + 2*F(n-1) */
}

/* a(n) = concatenation of the squares of the digits of Lucas(n) */
seq_t a(idx_t n)
{
    seq_t lucas = lucas_n(n);

    /* convert Lucas(n) to decimal string */
    size_t digs = digit_len(lucas);
    size_t buf_size = digs * 2 + 1;       /* maximum needed */
    char *buf = (char *)malloc(buf_size);
    if (!buf) return 0;                  /* allocation failure */
    size_t buf_len;
    seq_to_str(lucas, buf, &buf_len);     /* buf_len == digs */

    seq_t result = 0;
    for (size_t i = 0; i < buf_len; ++i) {
        int d = buf[i] - '0';
        seq_t sq = (seq_t)(d * d);        /* 0 .. 81 */
        char sq_buf[4];
        size_t sq_len;
        seq_to_str(sq, sq_buf, &sq_len);  /* sq_len is 1 or 2 */
        seq_t mult = pow10((idx_t)sq_len);
        result = result * mult + sq;
    }

    free(buf);
    return result;
}
