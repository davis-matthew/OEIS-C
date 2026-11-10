/*
 * A364850  a(n) = negated numerator of A014963(n)*A023900(n)/2.
 * https://oeis.org/A364850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   a(n) = -numerator(A014963(n)*A023900(n)/2), where A023900(n) =
 *            Product_{p|n} (1-p) and A014963(n) = p if n is a power of prime
 *            p, else 1
 * Range:     i8 n <= 12, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;   /* A023900(n) */
    seq_t lam = (seq_t)1;    /* A014963(n) */
    int distinct = 0;
    idx_t m = n;
    idx_t p;

    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            prod *= ((seq_t)1 - (seq_t)p);
            lam = (seq_t)p;
            distinct++;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1) {
        prod *= ((seq_t)1 - (seq_t)m);
        lam = (seq_t)m;
        distinct++;
    }
    if (distinct != 1)
        lam = (seq_t)1;

    seq_t x = lam * prod;
    if (x % 2 == 0)
        return -(x / 2);
    return -x;
}
