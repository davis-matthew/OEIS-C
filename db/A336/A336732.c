/*
 * A336732  The number of tight 4 X n pavings.
 * https://oeis.org/A336732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (4^(n+5)+(n-42)*3^(n+4)-9*(2*n-27)*2^(n+5)-36*n^3-486*n^2-2577*n-5398)/36
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* powers */
    seq_t p4 = pow4((idx_t)(n + 5));         /* 4^(n+5)   */
    seq_t p3 = pow3((idx_t)(n + 4));         /* 3^(n+4)   */
    seq_t p2 = pow2((idx_t)(n + 5));         /* 2^(n+5)   */

    /* polynomial powers of n */
    seq_t n_sq = n_seq * n_seq;              /* n^2       */
    seq_t n_cu = n_sq * n_seq;              /* n^3       */

    /* accumulate positive and negative parts separately to avoid
       signed arithmetic on unsigned seq_t types */
    seq_t pos = 0;   /* positive contributions */
    seq_t neg = 0;   /* negative contributions */

    /* term 4^(n+5) is always positive */
    pos += p4;

    /* term (n-42)*3^(n+4) */
    if (n >= 42) {
        pos += (n_seq - (seq_t)42) * p3;
    } else {
        neg += ((seq_t)42 - n_seq) * p3;
    }

    /* term -9*(2*n-27)*2^(n+5) */
    if (n >= 14) {          /* 2*n-27 >= 0  -> term is negative */
        neg += (seq_t)9 * (seq_t)(2 * n_seq - (seq_t)27) * p2;
    } else {                /* 2*n-27 < 0   -> term is positive */
        pos += (seq_t)9 * (seq_t)((seq_t)27 - 2 * n_seq) * p2;
    }

    /* polynomial terms: all negative */
    neg += (seq_t)36 * n_cu;
    neg += (seq_t)486 * n_sq;
    neg += (seq_t)2577 * n_seq;
    neg += (seq_t)5398;

    /* final numerator and division by 36 */
    seq_t numerator = pos - neg;          /* non‑negative by construction */
    return numerator / (seq_t)36;
}
