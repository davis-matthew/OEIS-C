/*
 * A341997  a(n) = A327936(A003415(n)).
 * https://oeis.org/A341997
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A327936(A003415(n)); A003415 = arithmetic derivative,
 *            A327936 multiplicative with a(p^e) = p if e >= p, else 1
 * Range:     u8 n <= 1414, u16 n <= 65539, u32 n <= 65539, u64 n <= 65539,
 *            u128 n <= 65539, big any n
 * Validated: standard, pass, 135 terms through n = 9933 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A003415_a(seq_t n) {
    if (n < 2)
        return 0;
    seq_t result = 0;
    seq_t rem = n;
    for (seq_t p = 2; p * p <= rem; p++) {
        int e = 0;
        while (rem % p == 0) {
            e++;
            rem /= p;
        }
        if (e > 0)
            result += (n / p) * (seq_t)e;
    }
    if (rem > 1)
        result += n / rem;
    return result;
}

static seq_t A327936_a(seq_t n) {
    if (n < 1)
        return 0;
    seq_t prod = 1;
    seq_t x = n;
    for (seq_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            seq_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            if (e >= p)
                prod *= p;
        }
    }
    return prod;
}

seq_t a(idx_t n)
{
    return A327936_a(A003415_a((seq_t)n));
}
