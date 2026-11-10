/*
 * A400674  First differences of A365873, divided by 3.
 * https://oeis.org/A400674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = (A365873(n+1) - A365873(n))/3, where A365873(1) = 1 and
 *            each next term is the least A365873(n)+3*(10*(last digit of
 *            A365873(n)) + f) whose first digit is f (f = 1..9); computed by
 *            direct generation
 * Range:     u8 n <= 2, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A365873_cache = NULL;
static size_t A365873_cap = 0;
static size_t A365873_len = 0;

static void A365873_grow(size_t need)
{
    if (need <= A365873_cap)
        return;
    size_t new_cap = A365873_cap ? A365873_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    A365873_cache = mem_resize(seq_t, A365873_cache, new_cap);
    A365873_cap = new_cap;
}

static seq_t A365873_next(seq_t prev)
{
    const seq_t ten = 10;
    seq_t last_digit = prev % ten;
    for (int f = 1; f <= 9; ++f) {
        seq_t f_seq = f;
        seq_t x = (seq_t)10 * last_digit + f_seq;
        seq_t cand = prev + (seq_t)3 * x;
        seq_t t = cand;
        while (t >= ten)
            t /= ten;
        if (t == f_seq)
            return cand;
    }
    return prev + (seq_t)3 * ((seq_t)10 * last_digit + (seq_t)1);
}

static seq_t A365873_a(idx_t n)
{
    A365873_grow((size_t)n);
    if (A365873_len == 0) {
        A365873_cache[0] = 1;
        A365873_len = 1;
    }
    while (A365873_len < (size_t)n) {
        seq_t nx = A365873_next(A365873_cache[A365873_len - 1]);
        A365873_cache[A365873_len++] = nx;
    }
    return A365873_cache[(size_t)n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t hi = A365873_a(n + 1);
    seq_t lo = A365873_a(n);
    return (hi - lo) / 3;
}
