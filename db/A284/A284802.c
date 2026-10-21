/*
 * A284802  Fixed points of the transform A284801.
 * https://oeis.org/A284802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth smallest positive integer whose base‑5 digits satisfy
 *            d_i + d_{m-1-i} = 4 for i = 0 … m-1 (m is the number of digits)
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned long long pow5_u64(idx_t exp) {
    unsigned long long r = 1ULL;
    while (exp--) r *= 5ULL;
    return r;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;          /* the only length‑1 solution is 2 */

    idx_t rem = n - 1;                    /* offset after the first term */
    idx_t len = 2;                        /* smallest length that has more than one term */

    /* find the proper length for the nth term */
    while (1) {
        idx_t klen = len / 2;             /* floor(m/2) */
        unsigned long long pow5 = 1ULL;
        for (idx_t i = 1; i < klen; ++i) pow5 *= 5ULL;
        unsigned long long count = 4ULL * pow5;   /* count of numbers of this length */

        if (rem <= count) break;
        rem -= count;
        ++len;
    }

    /* now len is the length of the desired number */
    idx_t klen = len / 2;
    unsigned long long pow5 = 1ULL;
    for (idx_t i = 1; i < klen; ++i) pow5 *= 5ULL;   /* 5^{klen-1} */
    unsigned long long highBase = pow5;              /* base for the high digits */
    unsigned long long idx0 = (unsigned long long)(rem - 1); /* 0‑based index in this length */

    unsigned long long hi    = idx0 / highBase;     /* 0..3  → leading digit 1..4 */
    unsigned long long lower = idx0 % highBase;     /* remaining part for the other high digits */

    seq_t digits[64] = {0};                         /* digits[0] = most significant digit */

    digits[0] = (seq_t)(hi + 1);                    /* leading digit */

    /* assign the remaining high‑half digits */
    unsigned long long digitPow = highBase;
    for (idx_t i = 1; i < klen; ++i) {
        digitPow /= 5ULL;                          /* next lower power of five */
        seq_t d = (seq_t)((lower / digitPow) % 5ULL);
        digits[i] = d;
    }

    /* middle digit for odd length is fixed to 2 */
    if (len % 2 == 1) digits[klen] = 2U;

    /* mirror the high half to the low half */
    for (int j = (int)len - 1; j >= (int)klen; --j) {
        if (len % 2 == 1 && j == (int)klen) continue; /* skip the middle digit */
        digits[j] = (seq_t)(4U - digits[(int)(len - 1 - j)]);
    }

    /* convert the digit array to a decimal value */
    seq_t value = 0;
    for (idx_t i = 0; i < len; ++i) {
        value = value * 5 + digits[i];
    }
    return value;
}
