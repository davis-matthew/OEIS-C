/*
 * A379040  Fixed points in A379015.
 * https://oeis.org/A379040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer whose non‑adjacent form (NAF) representation
 *            is a palindrome. Computed by testing successive integers
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Check whether the non‑adjacent form of 'num' is a palindrome.
   Returns 1 if true, 0 otherwise. */
static int naf_palindrome(seq_t num)
{
    if (num == 0) return 1;            /* 0 has an empty NAF, which is palindromic */
    int len = 0;
    /* A single padded array is enough for numbers up to 256 bits. */
    int8_t digits[200];
    seq_t t = num;
    while (t > 0) {
        if ((t & 1) != 0) {            /* odd */
            if ((t & 3) == 1) {        /* t ≡ 1 (mod 4) → digit = +1 */
                digits[len++] = 1;
                t = (t - 1) >> 1;      /* (t - 1)/2 */
            } else {                   /* t ≡ 3 (mod 4) → digit = -1 */
                digits[len++] = -1;
                t = (t + 1) >> 1;      /* (t + 1)/2 */
            }
        } else {                       /* even */
            digits[len++] = 0;
            t = t >> 1;
        }
    }
    /* Check palindrome */
    for (int i = 0; i < len / 2; ++i)
        if (digits[i] != digits[len - 1 - i]) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;  /* a(1) = 0 */

    idx_t count = 1;              /* already counted 0 */
    seq_t m = 1;                  /* start testing from 1 */

    for (;;) {
        if (naf_palindrome(m)) {
            ++count;
            if (count == n) return m;
        }
        ++m;
    }
}
