/*
 * A391473  a(n) is the smallest positive integer k such that k*R_n has digits
 *          with alternating parity, where R_n is the n-th repunit.
 * https://oeis.org/A391473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = smallest k such that k * R_n has alternating parity
 *            digits The sequence satisfies the closed forms (for n≥2): a(2*m)
 *            = (4060*10^(2*m-4) + 21)/11 a(2*m-1) = (208*10^(2*m-4) + 1 )/11
 *            A(1)=1, A(2)=11
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 1) return 1;
    if (n == 2) return 11;

    if ((n & 1) == 0) {               /* even index: n = 2*m */
        idx_t m   = n / 2;
        idx_t exp = 2 * m - 4;        /* exponent for 10^exp */
        seq_t t   = pow10(exp);
        seq_t num = mul_small(t, 4060);
        num = num + (seq_t)21;
        return num / 11;
    } else {                           /* odd index: n = 2*m-1 */
        idx_t m   = (n + 1) / 2;
        idx_t exp = 2 * m - 4;        /* exponent for 10^exp */
        seq_t t   = pow10(exp);
        seq_t num = mul_small(t, 208);
        num = num + (seq_t)1;
        return num / 11;
    }
}
