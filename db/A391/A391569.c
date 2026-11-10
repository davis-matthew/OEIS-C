/*
 * A391569  a(n) = n XOR A391573(n).
 * https://oeis.org/A391569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n XOR A391573(n), where A391573(n) = A048720(n,k)/k for
 *            the smallest odd k>1 with k | A048720(n,k); found by search
 * Range:     u8 n <= 46, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 111 terms through n = 9983 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A391573_a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t n_val = (seq_t)n;

    for (idx_t k = 3; ; k += 2) {
        seq_t prod = (seq_t)0;
        idx_t kk = k;
        idx_t shift = 0;

        while (kk != 0) {
            if (kk & 1ULL)
                prod ^= (n_val << shift);
            kk >>= 1;
            shift++;
        }

        if ((prod % (seq_t)k) == 0)
            return prod / (seq_t)k;
    }
}

seq_t a(idx_t n)
{
    return (seq_t)n ^ A391573_a(n);
}
