/*
 * A361150  a(n) = A014284(n^2) + A014284(n^2-1).
 * https://oeis.org/A361150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014284(n^2) + A014284(n^2-1), with A014284(m) = 1 + sum
 *            of first m-1 primes, A014284(0)=0; computed with a sieve as a(n)
 *            = 2 + 2*(sum of first n^2-1 primes) - prime(n^2-1), a(1)=1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= (idx_t)1) return (seq_t)1;
    idx_t need = n * n - 1;
    size_t limit = 64;
    for (;;) {
        uint8_t *comp = (uint8_t *)calloc(limit + 1, 1);
        if (!comp) return (seq_t)0;
        seq_t total = (seq_t)0;
        seq_t last = (seq_t)0;
        idx_t count = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (!comp[i]) {
                total += (seq_t)i;
                last = (seq_t)i;
                ++count;
                if (count == need) break;
                if ((uint64_t)i * i <= limit) {
                    for (size_t j = i * i; j <= limit; j += i) comp[j] = 1;
                }
            }
        }
        free(comp);
        if (count == need) {
            return (seq_t)2 + (seq_t)2 * total - last;
        }
        limit *= 2;
    }
}
