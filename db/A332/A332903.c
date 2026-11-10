/*
 * A332903  a(1) = 1, then after the first differences of A333123.
 * https://oeis.org/A332903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = A333123(n) - A333123(n-1) for n > 1, where
 *            A333123(1) = 1, A333123(n) = Sum_{p|n, p prime} A333123(n - n/p)
 * Range:     i8 n <= 104, i16 n <= 986, i32 n <= 10200, i64 n <= 10200,
 *            i128 n <= 10200, big any n
 * Validated: standard, pass, 107 terms through n = 8730 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A332903_F = NULL;
static size_t A332903_done = 0; /* values valid for indices 0..done-1 */

static void
A332903_ensure(idx_t n)
{
    if ((size_t)n < A332903_done) return;
    seq_t *tmp = mem_resize(seq_t, A332903_F, (size_t)n + 1);
    if (!tmp) abort();
    A332903_F = tmp;
    if (A332903_done == 0) {
        A332903_F[0] = 0;
        A332903_F[1] = 1;
        A332903_done = 2;
    }
    for (size_t m = A332903_done; m <= (size_t)n; ++m) {
        seq_t sum = 0;
        size_t t = m;
        for (size_t p = 2; p <= t / p; ++p) {
            if (t % p == 0) {
                sum += A332903_F[m - m / p];
                while (t % p == 0) t /= p;
            }
        }
        if (t > 1) sum += A332903_F[m - m / t];
        A332903_F[m] = sum;
    }
    A332903_done = (size_t)n + 1;
}

seq_t
a(idx_t n)
{
    if (n <= 1) return 1;
    A332903_ensure(n);
    return A332903_F[n] - A332903_F[n - 1];
}
