/*
 * A325428  Complement of A325427.
 * https://oeis.org/A325428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number that is not in A325427 (A325427 built by a
 *            sieve: k is in A325427 unless forbidden by 2x+1 or floor(3x/2)
 *            of an earlier member)
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9163 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t limit = (size_t)n * 2 + 16;
    for (;;) {
        size_t sz = limit * 2 + 4;
        unsigned char *f = (unsigned char *)calloc(sz, 1);
        if (!f) abort();
        size_t count = 0;
        size_t result = 0;
        for (size_t k = 1; k <= limit; ++k) {
            if (f[k]) {
                ++count;
                if (count == (size_t)n) { result = k; break; }
            } else {
                f[2 * k + 1] = 1;
                size_t t = (3 * k) / 2;
                if (t > k) f[t] = 1;
            }
        }
        free(f);
        if (result) return (seq_t)result;
        limit *= 2;
    }
}
