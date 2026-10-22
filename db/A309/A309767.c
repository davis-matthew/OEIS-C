/*
 * A309767  Approximation of the 2-adic integer arctanh(4) up to 2^n.
 * https://oeis.org/A309767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{i=0..floor((n-3)/4)} 4^(2*i+1)/(2*i+1)) mod 2^n
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* --- local helpers ------------------------------------------------------- */
static seq_t _inv_mod_pow2(seq_t a, idx_t n)
{
    /* Compute the inverse of an odd number a modulo 2^n.
       Uses Hensel lifting: x_{k+1} = x_k * (2 - a*x_k) (mod 2^{2k})   */
    seq_t inv = 1;          /* inverse modulo 2 (trivial) */
    idx_t k   = 1;          /* current precision in bits       */

    while (k < n) {
        /* modulus = 2^(2k) */
        seq_t mask = pow2(2 * k);
        mask -= 1;            /* 2^(2k) - 1 */

        seq_t prod = a * inv;
        prod &= mask;              /* modulo 2^(2k) */

        seq_t tmp = (seq_t)2 - prod; /* 2 - a*inv          */
        tmp &= mask;                /* modulo 2^(2k)      */

        inv = (inv * tmp) & mask;   /* update inverse     */
        k <<= 1;                    /* double current bits */
    }

    /* truncate to n bits */
    seq_t maskn = pow2(n);
    maskn -= 1;
    return inv & maskn;
}

/* --- required sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;        /* a(0)=a(1)=a(2)=0 */

    /* mask = 2^n - 1  (keeps only the low n bits)   */
    seq_t mask = pow2(n);
    mask -= 1;

    /* number of terms in the sum */
    idx_t m = (n > 3) ? (n - 3) / 4 : 0;

    seq_t sum      = 0;
    seq_t pow_term = pow2(2);  /* 4^(2*0+1) = 4 */
    for (idx_t i = 0; i <= m; ++i) {
        /* compute inverse of odd denominator (2*i+1) mod 2^n */
        seq_t inv = _inv_mod_pow2((seq_t)(2 * i + 1), n);

        /* term = 4^(2*i+1)/(2*i+1)  (mod 2^n) */
        seq_t term = (pow_term * inv) & mask;
        sum += term;
        sum &= mask;

        /* update pow_term for the next iteration:
           4^(2*(i+1)+1) = 4^(2*i+1) * 4^2 = pow_term * 16   */
        if (i < m) {
            pow_term = (pow_term * 16) & mask;
        }
    }
    return sum;
}
