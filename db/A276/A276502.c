/*
 * A276502  Least k > 0 such that A045876(n) divides A045876(n*10^k).
 * https://oeis.org/A276502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search for the least k > 0 with A045876(n) | A045876(n*10^k),
 *            where A045876(m) = R_D * S * (D-1)! / prod_d f_d! (R_D =
 *            (10^D-1)/9, D = #digits, S = digit sum, f_d = frequency of digit
 *            d). Appending k zeros gives A045876(n*10^k) = R_{D+k} * S *
 *            prod_{j=1..m-1}(f0+k+j) / P, with m = number of nonze...
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Validated: partial, pass, 101 terms through n = 101 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A276502_u128;

seq_t a(idx_t n)
{
    unsigned int freq[10] = {0,0,0,0,0,0,0,0,0,0};
    unsigned int D = 0;
    A276502_u128 S = 0;
    idx_t t = n;
    do {
        unsigned int d = (unsigned int)(t % 10);
        freq[d]++;
        S += d;
        t /= 10;
        D++;
    } while (t > 0);

    /* multinomial D! / prod f_d!  computed incrementally (exact) */
    A276502_u128 mult = 1, tot = 0;
    for (int d = 0; d < 10; ++d) {
        for (unsigned int i = 1; i <= freq[d]; ++i) {
            tot++;
            mult = mult * tot / i;
        }
    }
    A276502_u128 R = 0;
    for (unsigned int i = 0; i < D; ++i) R = R * 10 + 1;
    A276502_u128 M = R * (mult * S / D);   /* A045876(n) */

    /* P = prod over nonzero digits of f_d! */
    A276502_u128 P = 1;
    unsigned int m = 0;
    for (int d = 1; d < 10; ++d) {
        m += freq[d];
        for (unsigned int i = 2; i <= freq[d]; ++i) P *= i;
    }
    unsigned int f0 = freq[0];
    A276502_u128 Q = M * P;

    /* R_{D} mod M, extended as k grows */
    A276502_u128 Rm = 0;
    for (unsigned int i = 0; i < D; ++i) Rm = (Rm * 10 + 1) % M;

    for (uint64_t k = 1; k < 100000000ULL; ++k) {
        Rm = (Rm * 10 + 1) % M;                /* R_{D+k} mod M */
        /* Num = S * prod_{j=1}^{m-1} (f0 + k + j)  mod Q */
        A276502_u128 num = S % Q;
        for (unsigned int j = 1; j < m; ++j) {
            A276502_u128 fac = ((A276502_u128)f0 + k + j) % Q;
            num = (num * fac) % Q;
        }
        A276502_u128 Wm = (num / P) % M;       /* exact: P | numerator */
        if ((Rm * Wm) % M == 0) return (seq_t)k;
    }
    return 0;
}
