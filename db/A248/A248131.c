/*
 * A248131  Start with a(0) = 1, then a(n) = three times the n-th digit of the
 *          sequence.
 * https://oeis.org/A248131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A248131: a(0) = 1; for n > 0, a(n) = 3 × (n‑th digit of the
 * sequence.  The sequence of digits is obtained by writing each a(k)
 * in decimal and concatenating them; the n‑th digit (1‑based) of this
 * concatenation drives a(n).  The function memoizes the sequence so
 * that calls may be made in any order. */
#include <stdlib.h>
#include <stdint.h>

/* External types supplied by the build configuration */
seq_t a(idx_t n)
{
    /* static storage for the sequence values */
    static seq_t *seq      = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;

    /* static storage for the decimal digits of the sequence */
    static uint8_t *digits  = NULL;
    static size_t digits_cap = 0;
    static size_t digits_len = 0;

    /* ---------  initialise on first use  --------- */
    if (seq_len == 0) {
        seq_cap   = 16;
        seq       = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0]    = (seq_t)1;      /* a(0) = 1 */
        seq_len   = 1;

        digits_cap = 16;
        digits     = (uint8_t *)malloc(digits_cap);
        digits[0]  = 1;            /* first digit of a(0) */
        digits_len = 1;
    }

    /* ---------  expand the sequence up to n  --------- */
    while (seq_len <= n) {
        size_t cur        = seq_len;   /* index we are computing */
        uint8_t d = 0;

        /* the driving digit is the digit number cur in the digit
         * stream; digits are 0‑based here, so use digits[cur-1] when
         * cur > 0.  For cur == 0 the digit is irrelevant because
         * a(0) is defined. */
        if (cur > 0) {
            /* the digits array always contains at least cur digits
             * because we append the digits of every term we compute. */
            d = digits[cur - 1];
        }

        /* compute the term */
        seq_t val = (seq_t)3 * (unsigned)d;

        /* ensure capacity for the sequence array */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = val;

        /* ----------  append the decimal digits of val  ---------- */
        /* Count how many decimal digits val has.  Handle val == 0
         * explicitly (though this never occurs for this sequence). */
        seq_t temp = val;
        size_t needed = 0;
        if (temp == 0) {
            needed = 1;
        } else {
            while (temp != 0) {
                needed++;
                temp /= (seq_t)10;
            }
        }

        /* ensure space in the digit buffer */
        if (digits_len + needed > digits_cap) {
            digits_cap = digits_cap ? digits_cap << 1 : 4;
            while (digits_len + needed > digits_cap) digits_cap <<= 1;
            digits = (uint8_t *)realloc(digits, digits_cap);
        }

        /* write the digits of val in reverse order into a temporary
         * buffer, then copy them forward into the main digits array. */
        seq_t tmp = val;
        uint8_t rev[64];            /* 64 should be enough for any
                                     * seq_t that fits in uint64_t, but
                                     * the loop works even if rev is
                                     * slightly too small because we
                                     * copy only the needed digits. */
        size_t revpos = 0;
        if (tmp == 0) {
            rev[revpos++] = 0;
        } else {
            while (tmp != 0 && revpos < sizeof(rev)) {
                rev[revpos++] = (uint8_t)(tmp % (seq_t)10);
                tmp /= (seq_t)10;
            }
        }

        /* copy digits to the main array in the correct order */
        for (size_t i = 0; i < revpos; ++i) {
            digits[digits_len + i] = rev[revpos - 1 - i];
        }
        digits_len += revpos;
    }

    return seq[n];
}
