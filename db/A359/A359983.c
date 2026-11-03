/*
 * A359983  Numbers with exactly two nonzero decimal digits and not ending
 *          with 0.
 * https://oeis.org/A359983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A359983: Numbers with exactly two nonzero decimal digits and not ending with 0.
   The terms are all numbers of the form a*10^(k-1) + c,
   where 1 <= a <= 9, 1 <= c <= 9 and k >= 2.
   For each fixed length k, the numbers are listed in increasing order
   by a (most significant non‑zero digit) and then by c (least significant non‑zero digit).
   Since there are 9*9 = 81 numbers per digit length, the nth term can be found
   by locating the digit length and the (a,c) pair within that block.
*/

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1, but guard 0 */
    idx_t idx = n - 1;               /* zero‑based index of the term   */
    const idx_t GROUP = 81;          /* 9 digits for a * 9 digits for c */

    idx_t group_idx = idx / GROUP;   /* which length‑k block (k-2)     */
    idx_t pos = idx % GROUP;         /* position within the block     */
    idx_t k = 2 + group_idx;         /* number of decimal digits       */

    idx_t a_digit = pos / 9 + 1;     /* most significant non‑zero digit */
    idx_t c_digit = pos % 9 + 1;     /* least significant non‑zero digit */

    seq_t term = (seq_t)a_digit * pow10(k - 1) + (seq_t)c_digit;
    return term;
}
