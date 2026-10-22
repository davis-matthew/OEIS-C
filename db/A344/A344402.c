/*
 * A344402  a(n) = denominator(R(n,3)), where R(n,d) = (Product_{j prime to d}
 *          Pochhammer(j/d, n)) / n!.
 * https://oeis.org/A344402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^( (5*n - s₃(n))/2 ), where s₃(n) = sum of base‑3 digits
 *            of n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via exponentiation by squaring. */
/* `big_pow` from the utils library is used to raise 3 to the required exponent. */
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Compute sum of base‑3 digits of n */
    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        sum += (tmp % 3);
        tmp /= 3;
    }

    /* Exponent: (5*n - sum)/2 */
    seq_t tn = (seq_t)n;          /* cast to seq_t for arbitrary precision */
    seq_t five_n = 5 * tn;        /* 5*n */
    seq_t exp = (five_n - sum) / 2;  /* integer division; numerator is always even */

    /* Return 3^exp */
    return big_pow((seq_t)3, exp);
}
