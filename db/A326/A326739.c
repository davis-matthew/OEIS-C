/*
 * A326739  Numbers which converge to 6 under repeated application of the
 *          powertrain map of A133500.
 * https://oeis.org/A326739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers that converge to 6 under repeated application of the
 *            powertrain map of A133500
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow, digit_len, seq_alloc, seq_mul, seq_realloc, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_to_str.h"
#include "../utils/digit_len.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t powertrain(seq_t x)
{
    size_t len = digit_len(x);
    char *buf = (char *)malloc(len + 1);
    seq_t tmp;
    seq_to_str(x, buf, &len);   /* returns len in the third arg */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i + 1 < (idx_t)len; i += 2) {
        seq_t base = (seq_t)(buf[i] - '0');
        seq_t exp  = (seq_t)(buf[i + 1] - '0');
        seq_t pow  = big_pow(base, exp);
        result = seq_mul(result, pow);
    }
    if (len % 2 == 1) {
        seq_t last = (seq_t)(buf[len - 1] - '0');
        result = seq_mul(result, last);
    }
    free(buf);
    return result;
}

static bool converge_to_6(seq_t x)
{
    if (x == (seq_t)6) return true;
    seq_t tortoise = x;
    seq_t hare     = x;
    for (idx_t step = 0; step < 200; ++step) {   /* safety limit */
        tortoise = powertrain(tortoise);
        hare = powertrain(powertrain(hare));
        if (tortoise == (seq_t)6 || hare == (seq_t)6) return true;
        if (tortoise == hare) return false;        /* cycle detected */
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static size_t arr_cap = 0;
    static size_t arr_len = 0;
    static idx_t next = 1;

    if (arr == NULL) {
        arr_cap = 64;
        arr = seq_alloc(arr_cap);
    }

    while (arr_len < n) {
        if (converge_to_6((seq_t)next)) {
            if (arr_len == arr_cap) {
                size_t new_cap = arr_cap * 2;
                arr = seq_realloc(arr, arr_cap, new_cap);
                arr_cap = new_cap;
            }
            arr[arr_len++] = (seq_t)next;
        }
        ++next;
    }
    return arr[n - 1];
}
