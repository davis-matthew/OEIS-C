/*
 * A262327  Number of (n+1) X (3+1) 0..1 arrays with each row and column
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A262327 – Number of (n+1)×(3+1) 0..1 arrays with each
 * row and column divisible by 3, read as a binary number with top
 * and left being the most significant bits.
 *
 * Formula (OEIS):
 *   a(1) = 6
 *   For n ≥ 2:
 *     if n even:  a(n) = 3^(n-2) * (14 + 2^(n+2)) / 2
 *     if n odd :  a(n) = 3^(n-2) * (28 + 2^(n+2)) / 2
 *   Recurrence: a(n) = 6*a(n-1) + 9*a(n-2) - 54*a(n-3),
 *     with a(1)=6, a(2)=15, a(3)=90.
 */

#include <stdint.h>           /* for uintptr_t if needed */
/* no other headers are required – only C arithmetic operators */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) {
            result = result * base;
        }
        exp >>= (idx_t)1;
        if (exp)             /* avoid useless final square when exp==0 */
            base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) {
        return (seq_t)6;
    }

    /* 3^(n-2)  and  2^(n+2)   */
    seq_t pow3 = pow_seq((seq_t)3, n - (idx_t)2);
    seq_t pow2 = pow_seq((seq_t)2, n + (idx_t)2);

    seq_t add;
    if (n & (idx_t)1) {              /* n is odd */
        add = (seq_t)28 + pow2;
    } else {                          /* n is even */
        add = (seq_t)14 + pow2;
    }

    seq_t prod = pow3 * add;
    return prod / (seq_t)2;
}
