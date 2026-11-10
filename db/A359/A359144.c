/*
 * A359144  Indices k such that A359142(k) is negative.
 * https://oeis.org/A359144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 1 such that A359142(k) < 0; found by a linear
 *            search over k
 * Range:     u8 n <= 1, u16 n <= 1641, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9528 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

static bool A359144_is_neg(idx_t k)
{
    char t[40];
    char s_str[8];
    size_t tn = 0;
    unsigned sum = 0;
    idx_t x = k;
    char rev[24];
    size_t r = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        rev[r++] = (char)('0' + d);
        sum += d;
        x /= 10;
    }
    while (r > 0) t[tn++] = rev[--r];
    snprintf(s_str, sizeof(s_str), "%u", sum);
    size_t ts = strlen(s_str);
    char D = t[0];
    bool d_in_s = false;
    for (size_t i = 0; i < ts; ++i)
        if (s_str[i] == D) d_in_s = true;
    if (!d_in_s) return false;
    memcpy(t + tn, s_str, ts);
    size_t total = tn + ts;
    char u[40];
    size_t ui = 0;
    for (size_t i = 0; i < total; ++i)
        if (t[i] != D) u[ui++] = t[i];
    if (ui == 0 || u[0] != '0') return false;
    for (size_t i = 0; i < ui; ++i)
        if (u[i] != '0') return true;
    return false;
}

seq_t a(idx_t n)
{
    static idx_t last_n = 0;
    static idx_t last_k = 0;
    if (n < last_n || last_n == 0) {
        last_n = 0;
        last_k = 0;
    }
    idx_t cnt = last_n;
    idx_t k = last_k;
    while (cnt < n) {
        ++k;
        if (A359144_is_neg(k)) ++cnt;
    }
    last_n = cnt;
    last_k = k;
    return (seq_t)k;
}
