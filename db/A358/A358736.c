/*
 * A358736  a(n) is the number of appearances of (9*n + 4) in A358509.
 * https://oeis.org/A358736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k >= 0 with digitsum((3^k-1)/2) = 9*n+4
 *            (A358509(k) = 9n+4); computed by scanning k up to a heuristic
 *            bound ~ (9n+4)/2 + 200 + 5*sqrt(9n+4)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 91 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    uint64_t v = 9 * (uint64_t)n + 4;
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= v) ++r;
    uint64_t K = v / 2 + 200 + 5 * r;

    size_t cap = (size_t)(K / 2 + 64);
    uint8_t *p = (uint8_t *)malloc(cap);
    uint8_t *t = (uint8_t *)malloc(cap);
    size_t len = 1;
    p[0] = 1;
    uint64_t count = 0;

    for (uint64_t k = 0; k <= K; ++k) {
        if (k > 0) {
            unsigned carry = 0;
            for (size_t i = 0; i < len; ++i) {
                unsigned pr = (unsigned)p[i] * 3 + carry;
                p[i] = (uint8_t)(pr % 10);
                carry = pr / 10;
            }
            while (carry) {
                p[len++] = (uint8_t)(carry % 10);
                carry /= 10;
            }
        }
        /* (3^k - 1)/2 */
        for (size_t i = 0; i < len; ++i) t[i] = p[i];
        size_t i = 0;
        while (i < len) {
            if (t[i] > 0) { t[i]--; break; }
            t[i] = 9;
            ++i;
        }
        unsigned rem = 0;
        uint64_t sum = 0;
        for (size_t j = len; j > 0; --j) {
            unsigned val = rem * 10 + t[j - 1];
            unsigned d = val / 2;
            rem = val % 2;
            sum += d;
        }
        if (sum == v) ++count;
    }
    free(p);
    free(t);
    return (seq_t)count;
}
