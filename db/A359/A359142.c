/*
 * A359142  Let s = sum of digits of n, let t = decimal concatenation of n and
 *          s, let u be obtained by deleting all copies of the leading digit
 *          of t from t, if this digit occurs in s. Then if u has only zero
 *          digits, a(n) = 0; if u has leading digit 0 but not all its digits
 *          are 0, delete all leading 0's from...
 * https://oeis.org/A359142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 328, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A359142: a(n) computed from n by concatenating n with its digit sum,
   removing all copies of the leading digit of that concatenation if that
   digit appears in the sum, then handling leading zeros and all‑zero
   results as specified in the OEIS definition. */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Convert unsigned 64‑bit value to a decimal string.
   Buffer must be at least 21 bytes. */
static void uint64_to_str(idx_t val, char *buf)
{
    /* snprintf is safe here; idx_t is equivalent to uint64_t */
    snprintf(buf, 21, "%llu", (unsigned long long)val);
}

/* Parse a decimal string (no sign) into seq_t. */
static seq_t str_to_seq(const char *s)
{
    seq_t v = 0;
    while (*s) {
        v = v * 10 + (*s - '0');
        ++s;
    }
    return v;
}

seq_t a(idx_t n)
{
    /* Step 1: get decimal representation of n */
    char n_str[21];
    uint64_to_str(n, n_str);

    /* Step 2: compute digit sum s */
    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        sum += tmp % 10;
        tmp /= 10;
    }

    /* Convert sum to string */
    char s_str[4];
    snprintf(s_str, sizeof(s_str), "%llu", (unsigned long long)sum);

    /* Step 3: concatenate n and s to form t */
    size_t tn = strlen(n_str);
    size_t ts = strlen(s_str);
    char t[25];
    memcpy(t, n_str, tn);
    memcpy(t + tn, s_str, ts);
    t[tn + ts] = '\0';

    /* Step 4: determine leading digit D */
    char D = t[0];

    /* Check whether D occurs in s_str */
    bool d_in_s = false;
    for (const char *p = s_str; *p; ++p) {
        if (*p == D) {
            d_in_s = true;
            break;
        }
    }

    /* Step 5: build u by deleting all copies of D from t if allowed */
    char u[25];
    if (d_in_s) {
        size_t ui = 0;
        for (size_t i = 0; i < tn + ts; ++i) {
            if (t[i] != D)
                u[ui++] = t[i];
        }
        u[ui] = '\0';
    } else {
        memcpy(u, t, tn + ts + 1);  /* include NUL */
    }

    /* Step 6: interpret u and return a(n) */
    if (u[0] == '\0')
        return (seq_t)0;              /* empty string -> 0 */

    /* Check all zeros */
    bool all_zeros = true;
    for (size_t i = 0; u[i] != '\0'; ++i) {
        if (u[i] != '0') {
            all_zeros = false;
            break;
        }
    }
    if (all_zeros)
        return (seq_t)0;

    /* Leading zero case: trim and negate */
    if (u[0] == '0') {
        size_t pos = 0;
        while (u[pos] == '0')
            ++pos;
        seq_t val = str_to_seq(u + pos);
        return -val;
    }

    /* Normal case */
    return str_to_seq(u);
}
