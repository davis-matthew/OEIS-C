/*
 * A277635  Number of 7's appearing in the sequence of consecutive natural
 *          numbers from 1 to A007908(n), where A007908 = (1, 12, 123, 1234,
 *          ...).
 * https://oeis.org/A277635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of digit '7' occurring in all integers from 1
 *              to A007908(n), where A007908(n) = concat(1..n)
 *
 * This implementation builds the decimal representation of the number
 * formed by concatenating 1 to n, then counts the digit '7' using the
 * standard digit‐frequency formula for an arbitrary large N.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

/* helper: convert an unsigned integer to decimal string */
static void uint_to_str(idx_t num, char *buf, size_t *len)
{
    if (num == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[32];
    size_t pos = 0;
    while (num > 0) {
        tmp[pos++] = '0' + (num % 10);
        num /= 10;
    }
    /* reverse */
    *len = pos;
    for (size_t i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
}

/* main function */
seq_t a(idx_t n)
{
    /* 1. Build decimal representation of 1..n (concatenation) */
    const size_t BUF_CHUNK = 1000;
    char *buf = NULL;
    size_t buf_cap = 0;
    size_t buf_len = 0;

    for (idx_t i = 1; i <= n; ++i) {
        char dig[32];
        size_t diglen;
        uint_to_str(i, dig, &diglen);
        if (buf_len + diglen > buf_cap) {
            size_t new_cap = buf_cap ? buf_cap * 2 : BUF_CHUNK;
            while (new_cap < buf_len + diglen)
                new_cap *= 2;
            char *new_buf = (char *)realloc(buf, new_cap);
            if (!new_buf) { /* allocation failed: return 0 */
                free(buf);
                return (seq_t)0;
            }
            buf = new_buf;
            buf_cap = new_cap;
        }
        memcpy(buf + buf_len, dig, diglen);
        buf_len += diglen;
    }

    /* 2. Count digit '7' in the huge number using the digit‑frequency
       formula. Works directly on the string representation. */
    seq_t result = 0;
    seq_t pow10_left = 1;     /* = 10^rightLen for current position */
    seq_t suffix_current = 0; /* numeric value of digits to the right */

    for (size_t idx = buf_len; idx > 0; /* no increment */ ) {
        --idx;
        unsigned d = (unsigned)(buf[idx] - '0');
        size_t rightLen = buf_len - 1 - idx;

        /* term for digit position (if rightLen > 0) */
        if (rightLen > 0) {
            seq_t pow10_prev = pow10_left / 10;          /* 10^(rightLen-1) */
            seq_t term = mul_small(pow10_prev, (unsigned)rightLen);
            term = mul_small(term, d);
            result += term;
        }

        /* leading digit contributes when d > 7 */
        if (d > 7) {
            result += pow10_left;
        }
        /* contributions from the current digit being 7 */
        if (d == 7) {
            result += suffix_current + 1;
        }

        /* update suffix and pow10_left for next iteration */
        seq_t part = mul_small(pow10_left, d);      /* d * 10^rightLen */
        suffix_current = suffix_current + part;
        pow10_left = mul_small(pow10_left, 10);
    }

    free(buf);
    return result;
}
