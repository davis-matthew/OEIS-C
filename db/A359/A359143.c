/*
 * A359143  The sum-and-erase sequence starting at 11: a(0) = 11; for n>=1,
 *          let m = a(n-1), and if m < 0, change m to an improper decimal
 *          "number" by replacing the minus sign by a single leading zero;
 *          then a(n) = A359142(m).
 * https://oeis.org/A359143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(0) = 11; a(n) = A359142(m) where m = a(n-1), with a negative m
 *            read as its digits preceded by a single 0 (iterated digit-string
 *            process, simulated directly)
 * Range:     i8 n <= 1, i16 n <= 5, i32 n <= 7, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define A359143_CAP 20000

static char A359143_d[A359143_CAP];   /* current digits, no leading zeros */
static size_t A359143_len = 0;
static bool A359143_neg = false;
static idx_t A359143_cn = 0;
static bool A359143_init = false;

static void A359143_reset(void)
{
    A359143_d[0] = '1'; A359143_d[1] = '1'; A359143_d[2] = '\0';
    A359143_len = 2;
    A359143_neg = false;
    A359143_cn = 0;
    A359143_init = true;
}

static void A359143_step(void)
{
    static char t[A359143_CAP + 64];
    static char u[A359143_CAP + 64];
    size_t tn = 0;
    if (A359143_neg) t[tn++] = '0';
    int sum = 0;
    for (size_t i = 0; i < A359143_len; i++) {
        t[tn++] = A359143_d[i];
        sum += A359143_d[i] - '0';
    }
    char sb[16];
    snprintf(sb, sizeof(sb), "%d", sum);
    size_t sl = strlen(sb);
    memcpy(t + tn, sb, sl);
    tn += sl;
    t[tn] = '\0';
    char D = t[0];
    bool inS = false;
    for (size_t i = 0; i < sl; i++) if (sb[i] == D) inS = true;
    size_t un = 0;
    for (size_t i = 0; i < tn; i++) {
        if (inS && t[i] == D) continue;
        u[un++] = t[i];
    }
    size_t p = 0;
    while (p < un && u[p] == '0') p++;
    if (p == un) {
        A359143_d[0] = '0'; A359143_d[1] = '\0';
        A359143_len = 1;
        A359143_neg = false;
    } else {
        A359143_neg = (p > 0);
        A359143_len = un - p;
        if (A359143_len >= A359143_CAP) A359143_len = A359143_CAP - 1;
        memcpy(A359143_d, u + p, A359143_len);
        A359143_d[A359143_len] = '\0';
    }
    A359143_cn++;
}

seq_t a(idx_t n)
{
    if (!A359143_init || n < A359143_cn) A359143_reset();
    while (A359143_cn < n) A359143_step();
    seq_t v = 0;
    for (size_t i = 0; i < A359143_len; i++)
        v = v * 10 + (A359143_d[i] - '0');
    if (A359143_neg) v = -v;
    return v;
}
