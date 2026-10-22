/*
 * A332170  a(n) = 7*(10^(2n+1)-1)/9 - 7*10^n.
 * https://oeis.org/A332170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*(10^(2n+1)-1)/9 - 7*10^n (OEIS A332170)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast binary exponentiation of 10^e.
   Uses only seq_t arithmetic (multiplication, division, subtraction). */
static seq_t pow10_uint(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base    = (seq_t)10;
    while (e > 0) {
        if (e & 1U) result = result * base;
        base = base * base;
        e >>= 1U;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(0) = 0  (handled naturally by formula) */
    seq_t pow_2n1 = pow10_uint((idx_t)(2ULL * n + 1ULL));   /* 10^(2n+1) */
    seq_t pow_n   = pow10_uint(n);                         /* 10^n        */

    seq_t term1 = (pow_2n1 - (seq_t)1) / (seq_t)9;          /* (10^(2n+1)-1)/9 */
    term1 = term1 * (seq_t)7;                               /* *7            */
    seq_t term2 = pow_n * (seq_t)7;                         /* 7*10^n       */

    return term1 - term2;                                   /* final result */
}
