/*
 * A244559  Numbers obtained by concatenating the squares of the digits of n!.
 * https://oeis.org/A244559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A048385(A000142(n))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = concatenate( (digit of n!)^2 ) for each decimal digit of n! */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static const char *sq_str[10] = {
    "0", "1", "4", "9", "16", "25", "36", "49", "64", "81"
};

seq_t a(idx_t n)
{
    /* factorial of n */
    seq_t fact = factorial(n);

    /* convert factorial to decimal string */
    size_t fact_len = num_digits(fact);           /* number of digits in fact */
    char *fact_str = (char *)malloc(fact_len + 1);  /* +1 for null terminator */
    if (!fact_str) return 0;                       /* allocation failure */

    size_t used_len = 0;
    seq_to_str(fact, fact_str, &used_len);
    fact_str[used_len] = '\0';

    /* Build concatenated squares string */
    size_t max_out_len = used_len * 2 + 1;         /* worst case, each digit gives 2 chars */
    char *out_str = (char *)malloc(max_out_len);
    if (!out_str) { free(fact_str); return 0; }

    size_t out_pos = 0;
    for (size_t i = 0; i < used_len; ++i) {
        char ch = fact_str[i];
        if (ch < '0' || ch > '9') continue;      /* safety, should not happen */
        uint8_t d = (uint8_t)(ch - '0');
        const char *sq = sq_str[d];
        size_t sq_len = strlen(sq);
        memcpy(out_str + out_pos, sq, sq_len);
        out_pos += sq_len;
    }
    out_str[out_pos] = '\0';

    /* Convert the resulting string back to seq_t */
    seq_t result = seq_from_str(out_str);

    /* Clean up */
    free(out_str);
    free(fact_str);

    return result;
}
