/*
 * A332551  a(0)=0; thereafter a(n+1) = a(n) + A332550(a(n)).
 * https://oeis.org/A332551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(n+1) = a(n) + A332550(a(n)), where A332550(m) =
 *            10*max_digit_frequency(m) - digits(m)
 * Range:     u8 n <= 26, u16 n <= 5722, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8912 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static int A332550_step(seq_t m)
{
    int freq[10] = {0};
    int len = 0;
    if (m == 0) {
        freq[0] = 1;
        len = 1;
    } else {
        while (m > 0) {
            int d = (int)(long long)(m % 10);
            freq[d]++;
            m /= 10;
            len++;
        }
    }
    int mx = 0;
    for (int d = 0; d < 10; ++d)
        if (freq[d] > mx) mx = freq[d];
    return 10 * mx - len;
}

seq_t a(idx_t n)
{
    seq_t x = 0;
    for (idx_t i = 0; i < n; ++i) {
        x += (seq_t)A332550_step(x);
    }
    return x;
}
