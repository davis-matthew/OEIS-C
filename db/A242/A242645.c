/*
 * A242645  a(n) = concatenation of decimal expansions of powers of 11 (in
 *          decreasing order).
 * https://oeis.org/A242645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of 11^n, 11^(n-1), …, 11, 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   digit_len, seq_from_str, seq_mul, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_to_str.h"
#include "../utils/seq_from_str.h"
#include "../utils/digit_len.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Step 1: compute digit lengths of 11^k for k = 0..n and total length. */
    size_t *len_arr = (size_t*)calloc((size_t)n + 1, sizeof(size_t));
    if (!len_arr) return (seq_t)0;
    seq_t pow = (seq_t)1;
    size_t total_len = 0;
    for (idx_t k = 0; k <= n; ++k) {
        size_t d = (size_t)digit_len(pow);
        len_arr[(size_t)k] = d;
        total_len += d;
        if (k != n) pow = seq_mul(pow, (seq_t)11);
    }

    /* Step 2: compute decimal strings of each power. */
    char **str_arr = (char**)malloc((size_t)(n + 1) * sizeof(char*));
    if (!str_arr) { free(len_arr); return (seq_t)0; }
    pow = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k) {
        size_t d = len_arr[(size_t)k];
        char *s = (char*)malloc(d + 1);
        size_t len_out = 0;
        seq_to_str(pow, s, &len_out);
        s[d] = '\0';
        str_arr[(size_t)k] = s;
        if (k != n) pow = seq_mul(pow, (seq_t)11);
    }

    /* Step 3: build final concatenated string in descending order. */
    char *final = (char*)malloc(total_len + 1);
    size_t pos = 0;
    for (idx_t k = n; ; --k) {
        size_t d = len_arr[(size_t)k];
        memcpy(final + pos, str_arr[(size_t)k], d);
        pos += d;
        if (k == 0) break;
    }
    final[total_len] = '\0';

    seq_t result = seq_from_str(final);

    /* Clean up. */
    free(final);
    for (idx_t k = 0; k <= n; ++k) free(str_arr[(size_t)k]);
    free(str_arr);
    free(len_arr);

    return result;
}
