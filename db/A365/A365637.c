/*
 * A365637  a(n) is the smallest number k such that k*n is a term of A072873.
 * https://oeis.org/A365637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = A365636(n)/n. Multiplicative with a(p^e) = p^(p*ceil(e/p)
 *            - e) for each prime power p^e
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* factor 2 */
    if ((m & 1) == 0) {
        idx_t cnt = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            cnt++;
        }
        idx_t eprime = 2 * ((cnt + 1) / 2) - cnt;   /* 2*ceil(cnt/2)-cnt */
        result = seq_mul(result, seq_pow((seq_t)2, eprime));
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            idx_t eprime = p * ((cnt + p - 1) / p) - cnt;  /* p*ceil(cnt/p)-cnt */
            result = seq_mul(result, seq_pow((seq_t)p, eprime));
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        idx_t p = m;                 /* remaining prime */
        idx_t eprime = p - 1;        /* cnt = 1 -> p*ceil(1/p)-1 = p-1 */
        result = seq_mul(result, seq_pow((seq_t)p, eprime));
    }

    return result;
}
