/*
 * A361350  A variant of A359143 which includes the intermediate terms before
 *          digits are deleted (see Comments for precise definition).
 * https://oeis.org/A361350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, fini
 * Formula:   simulate the digit-string process of A359143 starting at
 *            a(0)=11. From the current term, take T = S concatenated with
 *            digit-sum(S), where S is the digits of the term with any minus
 *            sign replaced by 0. If T's leading digit D is not in the digit
 *            sum, the next term is T with leading zeros stripped,...
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 25, i128 n <= 39,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 9851 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define A361350_CAP 200000

static char A361350_d[A361350_CAP];
static size_t A361350_len = 0;
static bool A361350_neg = false;
static idx_t A361350_bi = 0;
static bool A361350_init = false;

static char A361350_t[A361350_CAP + 64];
static char A361350_u[A361350_CAP + 64];
static size_t A361350_tn = 0;
static char A361350_D = '0';
static bool A361350_inS = false;

static void A361350_reset(void)
{
    A361350_d[0] = '1'; A361350_d[1] = '1'; A361350_d[2] = '\0';
    A361350_len = 2;
    A361350_neg = false;
    A361350_bi = 0;
    A361350_init = true;
}

static void A361350_buildT(void)
{
    size_t tn = 0;
    if (A361350_neg) A361350_t[tn++] = '0';
    int sum = 0;
    for (size_t i = 0; i < A361350_len; i++) {
        A361350_t[tn++] = A361350_d[i];
        sum += A361350_d[i] - '0';
    }
    char sb[16];
    snprintf(sb, sizeof(sb), "%d", sum);
    size_t sl = strlen(sb);
    memcpy(A361350_t + tn, sb, sl);
    tn += sl;
    A361350_t[tn] = '\0';
    A361350_tn = tn;
    A361350_D = A361350_t[0];
    A361350_inS = false;
    for (size_t i = 0; i < sl; i++)
        if (sb[i] == A361350_D) A361350_inS = true;
}

/* Convert digit string s (length n) to value, leading zeros -> negative. */
static seq_t A361350_value(const char *s, size_t n)
{
    size_t p = 0;
    while (p < n && s[p] == '0') p++;
    if (p == n) return (seq_t)0;
    seq_t v = 0;
    for (size_t i = p; i < n; i++) v = v * 10 + (seq_t)(s[i] - '0');
    if (p > 0) v = -v;
    return v;
}

static void A361350_step(void)
{
    A361350_buildT();
    size_t un = 0;
    for (size_t i = 0; i < A361350_tn; i++) {
        if (A361350_inS && A361350_t[i] == A361350_D) continue;
        A361350_u[un++] = A361350_t[i];
    }
    size_t p = 0;
    while (p < un && A361350_u[p] == '0') p++;
    if (p == un) {
        A361350_d[0] = '0'; A361350_d[1] = '\0';
        A361350_len = 1;
        A361350_neg = false;
    } else {
        A361350_neg = (p > 0);
        A361350_len = un - p;
        if (A361350_len >= A361350_CAP) A361350_len = A361350_CAP - 1;
        memcpy(A361350_d, A361350_u + p, A361350_len);
        A361350_d[A361350_len] = '\0';
    }
    A361350_bi += A361350_inS ? 2 : 1;
}

seq_t a(idx_t n)
{
    if (!A361350_init || n < A361350_bi) A361350_reset();
    for (;;) {
        A361350_buildT();
        idx_t nb = A361350_bi + (A361350_inS ? 2 : 1);
        if (nb <= n) A361350_step();
        else break;
    }
    if (n == A361350_bi) {
        seq_t v = 0;
        for (size_t i = 0; i < A361350_len; i++)
            v = v * 10 + (seq_t)(A361350_d[i] - '0');
        if (A361350_neg) v = -v;
        return v;
    }
    /* intermediate term: T itself */
    A361350_buildT();
    return A361350_value(A361350_t, A361350_tn);
}
