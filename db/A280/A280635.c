/*
 * A280635  Numbers whose digit string can be partitioned into three nonempty
 *          parts such that the product of the first two parts equals the
 *          third part.
 * https://oeis.org/A280635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A280635: numbers whose decimal representation can be split
 * into three non‑empty parts such that the product of the first two
 * parts equals the third.
 * 
 * Computation: brute‑force enumeration with digit extraction using
 * division by powers of ten.  For each candidate number we test all
 * admissible splits; if any split satisfies the condition the number
 * is accepted.
 */
 
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
 
#include "../utils/digit_len.h"   /* int digit_len(seq_t x); */
 
/* Check whether n satisfies the definition of A280635. */
static bool is_area_num(seq_t n)
{
    int len = digit_len(n);           /* number of decimal digits */
    if (len < 3) return false;        /* need at least three parts */
 
    /* Pre‑compute powers of ten up to len (10^k).  200 digits are
       sufficient for the expected size of the numbers for all n ≤ 10000. */
    static seq_t pow_buf[200];
    pow_buf[0] = (seq_t)1;
    for (int k = 1; k <= len; ++k)
        pow_buf[k] = pow_buf[k - 1] * (seq_t)10;
 
    /* Test all possible splits: first part length = i, second part
       length = j.  The digit at the beginning of each part must not
       be zero. */
    for (int i = 1; i + 1 < len; ++i) {
        seq_t left  = n / pow_buf[len - i];
        /* leading digit of second part */
        if ((n / pow_buf[len - i - 1]) % (seq_t)10 == (seq_t)0)
            continue;
        seq_t rest = n % pow_buf[len - i];
        for (int j = 1; i + j < len; ++j) {
            /* leading digit of third part */
            if ((rest / pow_buf[len - i - j - 1]) % (seq_t)10 == (seq_t)0)
                continue;
            seq_t mid   = rest / pow_buf[len - i - j];
            seq_t thr   = rest % pow_buf[len - i - j];
            if (left * mid == thr)
                return true;
        }
    }
    return false;
}
 
/* Return the n-th element (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n == 0 */
 
    idx_t count = 0;
    seq_t candidate = (seq_t)111;         /* smallest valid number */
 
    for (;;) {
        if (is_area_num(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate = candidate + (seq_t)1;
    }
}
