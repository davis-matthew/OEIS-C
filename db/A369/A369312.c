/*
 * A369312  Square array A(n, k), n >= 0, k > 0, read by upwards
 *          antidiagonals: P(A(n, k)) is the quotient of the polynomial long
 *          division of P(n) by P(k) (where P(m) denotes the polynomial over
 *          GF(2) whose coefficients are encoded in the binary expansion of
 *          the nonnegative integer m).
 * https://oeis.org/A369312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = P(n)/P(k) over GF(2), where (n,k) are obtained by *
 *            reading the square array A(n,k) by upward antidiagonals. The
 *            array * entry A(n,k) is the quotient polynomial of P(n) by P(k)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

/* number of bits in x (0 for x==0) */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x != 0) {
        x >>= 1;
        len++;
    }
    return len;
}

/* quotient of polynomial long division of P(n) by P(k) over GF(2) */
static seq_t poly_div_quos(seq_t num, seq_t den)
{
    if (den == 0) return 0;                 /* k > 0, so not expected */
    seq_t q = 0;
    unsigned deg_num = bitlen_seq_t(num);
    unsigned deg_den = bitlen_seq_t(den);
    while (deg_num >= deg_den) {
        unsigned shift = deg_num - deg_den;
        q ^= (seq_t)1 << shift;
        num ^= den << shift;
        if (num == 0) break;
        deg_num = bitlen_seq_t(num);
    }
    return q;
}

seq_t a(idx_t n)
{
    /* diagonal index s: smallest s such that n < (s+1)(s+2)/2 */
    double dn = (double)n;
    idx_t s = (idx_t)((sqrt(8.0 * dn + 1.0) - 1.0) / 2.0);
    idx_t t_s = s * (s + 1) / 2;        /* starting index of diagonal s */
    idx_t offset = n - t_s;             /* 0 <= offset < s+1 */
    idx_t n_val = s - offset;           /* row index of A(n,k) */
    idx_t k_val = offset + 1;           /* column index of A(n,k) */
    return poly_div_quos((seq_t)n_val, (seq_t)k_val);
}
