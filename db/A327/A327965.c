/*
 * A327965  "Tamed variant" of arithmetic derivative: a(0) = a(1) = 0; for n >
 *          1, a(n) = A327938(A003415(n)).
 * https://oeis.org/A327965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 0; for n > 1, a(n) = A327938(A003415(n)), where
 *            A327938 is multiplicative with a(p^e) = p^(e mod p)
 * Range:     u8 n <= 233, u16 n <= 19999, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 109 terms through n = 9620 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A327965_deriv(seq_t n)
{
    if (n < 2) return 0;
    seq_t result = 0;
    seq_t rem = n;
    for (seq_t p = 2; p <= rem / p; p++) {
        int e = 0;
        while (rem % p == 0) {
            e++;
            rem /= p;
        }
        if (e > 0) result += (n / p) * (seq_t)e;
    }
    if (rem > 1) result += n / rem;
    return result;
}

static seq_t A327965_tame(seq_t m)
{
    if (m < 1) return 0;
    seq_t result = 1;
    seq_t rem = m;
    for (seq_t p = 2; p <= rem / p; p++) {
        if (rem % p == 0) {
            seq_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                e++;
            }
            seq_t r = e % p;
            for (seq_t i = 0; i < r; i++) result *= p;
        }
    }
    if (rem > 1) result *= rem; /* exponent 1, 1 mod rem = 1 */
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    return A327965_tame(A327965_deriv((seq_t)n));
}
