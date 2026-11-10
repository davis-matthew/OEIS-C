/*
 * A368782  Comma transform of A366487.
 * https://oeis.org/A368782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*(c(n) mod 10) + first_digit(c(n+1)), where c(n) =
 *            A121805(n+1) - A121805(n) (A366487), and A121805 is the comma
 *            sequence, a(1) = 1, a(n) = a(n-1) + 10*lastdigit(a(n-1)) + y,
 *            with y the least digit equal to the leading digit of a(n)
 * Range:     u8 n <= 6, u16 n <= 1575, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 81 terms through n = 9970 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A368782_cache = NULL;
static idx_t A368782_max = 0;

static seq_t A368782_msd(seq_t x)
{
    while (x >= 10) x /= 10;
    return x;
}

static seq_t A368782_S(idx_t n)
{
    if (n > A368782_max) {
        A368782_cache = mem_resize(seq_t, A368782_cache, (size_t)(n + 1));
        if (A368782_max == 0) {
            A368782_cache[0] = 0;
            A368782_cache[1] = 1;
            A368782_max = 1;
        }
        for (idx_t i = A368782_max + 1; i <= n; ++i) {
            seq_t prev = A368782_cache[i - 1];
            seq_t base = prev + (prev % 10) * 10;
            seq_t val = 0;
            for (seq_t d = 0; d <= 9; ++d) {
                seq_t cand = base + d;
                if (A368782_msd(cand) == d) { val = cand; break; }
            }
            A368782_cache[i] = val;
        }
        A368782_max = n;
    }
    return A368782_cache[n];
}

static seq_t A368782_C1(idx_t n)
{
    return A368782_S(n + 1) - A368782_S(n);
}

seq_t a(idx_t n)
{
    seq_t c0 = A368782_C1(n);
    seq_t c1 = A368782_C1(n + 1);
    return (c0 % 10) * 10 + A368782_msd(c1);
}
