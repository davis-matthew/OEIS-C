/*
 * A260853  Base-3 representation of a(n) is the concatenation of the base-3
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the base‑3 representations of 1,2,…,n,
 *            n‑1,…,2,1 (a(0)=0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: append the base‑3 digits of a positive integer 'num'
   to the running result 'res' (most significant digit first). */
static void append_base3_digits(seq_t *res, uint64_t num)
{
    /* find the highest power of 3 <= num */
    uint64_t power = 1;
    while (power * 3ULL <= num)
        power *= 3ULL;

    for (; power > 0; power /= 3ULL) {
        uint64_t digit = num / power;          /* digit is 0,1,2 */
        *res = (*res) * (seq_t)3 + (seq_t)digit;
        num -= digit * power;
    }
}

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)0;

    seq_t result = 0;

    /* forward sequence 1..n */
    for (uint64_t i = 1; i <= n; ++i)
        append_base3_digits(&result, i);

    /* backward sequence n‑1..1 */
    for (uint64_t i = n - 1; i >= 1; --i) {
        append_base3_digits(&result, i);
        if (i == 1) /* avoid underflow of unsigned loop */
            break;
    }

    return result;
}
