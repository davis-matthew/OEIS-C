/*
 * A270142  a(n) = product of first k composites, with the i-th composite
 *          raised to the d-th power, where k = A055642(n) and d is the i-th
 *          digit of n.
 * https://oeis.org/A270142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product_{i=1}^{k} C_i^{d_i}, where C_i is the i‑th
 *            composite (4,6,8,9,10,...) and d_i is the i‑th decimal digit of
 *            n; k = number of digits of n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 58, u64 n <= 798,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits_of.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

static const seq_t composites[20] = {
    (seq_t)4, (seq_t)6, (seq_t)8, (seq_t)9, (seq_t)10,
    (seq_t)12,(seq_t)14,(seq_t)15,(seq_t)16,(seq_t)18,
    (seq_t)20,(seq_t)21,(seq_t)22,(seq_t)24,(seq_t)25,
    (seq_t)26,(seq_t)27,(seq_t)28,(seq_t)30,(seq_t)32
};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* undefined for 0; safe fallback */

    int k = digits_of(n);                 /* number of decimal digits */
    seq_t result = (seq_t)1;

    /* Store digits in reverse order (least significant first). */
    int rev[20];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        rev[len++] = (int)(tmp % 10);
        tmp /= 10;
    }

    for (int i = 0; i < k; ++i) {
        int digit = rev[k - 1 - i];       /* most significant digit first */
        seq_t base = composites[i];
        if (digit == 0) continue;        /* base^0 = 1 */
        seq_t powv = seq_pow(base, (idx_t)digit);
        result = seq_mul(result, powv);
    }
    return result;
}
