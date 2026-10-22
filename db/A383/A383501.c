/*
 * A383501  Nonnegative integers without the digit 9 such that every odd digit
 *          except the leftmost is immediately preceded by a larger digit.
 * https://oeis.org/A383501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th nonnegative integer that:
 *  – contains no digit 9
 *  – for every odd digit except the leftmost one, the immediately
 *    preceding digit is larger.
 * The implementation enumerates integers from 0 upwards and
 * checks the above property.  The search stops when the n‑th
 * qualifying integer has been found.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

static bool is_valid(seq_t x)
{
    if (x == 0)
        return true;

    /* buffer large enough for practical seq_t values (maximum
       39 decimal digits for 128‑bit integers; 512 covers even
       arbitrary‑precision values that appear in the test set). */
    uint8_t d[512];
    size_t len = 0;
    seq_t tmp = x;

    /* extract digits (least significant first) and reject numbers
       containing digit 9 during the extraction itself */
    while (tmp > 0) {
        uint8_t digit = (uint8_t)(tmp % 10);
        if (digit == 9)
            return false;          /* digit 9 is forbidden */
        d[len++] = digit;
        tmp /= 10;
    }

    /* scan from most significant to least significant digit */
    for (int i = (int)len - 1; i >= 0; --i) {
        uint8_t cur = d[i];
        if ((cur & 1) && i != (int)len - 1) { /* odd and not leftmost */
            uint8_t left = d[i + 1];          /* preceding digit */
            if (left <= cur)
                return false;                 /* must be larger */
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;                 /* a(1) = 0 */

    idx_t count = 1;              /* count already includes 0 */
    seq_t cur = 1;                /* start checking from 1 */

    while (1) {
        if (is_valid(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
