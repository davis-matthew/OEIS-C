/*
 * A275953  First differences of A265906; second differences of A265905.
 * https://oeis.org/A275953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A265906(n+1) - A265906(n), where A265906(n) =
 *            A265905(n+1) - A265905(n) = A153880(A265905(n)); A265905(1) = 1,
 *            A265905(n) = A265905(n-1) + A153880(A265905(n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 52 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A153880_h(seq_t x)
{
    if (x == 0) return (seq_t)0;
    seq_t fact = (seq_t)1;
    int k = 1;
    while (1) {
        seq_t next_fact = fact * (seq_t)(k + 1);
        if (next_fact > x) break;
        fact = next_fact;
        k++;
    }
    seq_t sum = (seq_t)0;
    seq_t rem = x;
    for (int cur = k; cur >= 1; --cur) {
        seq_t next_fact = fact * (seq_t)(cur + 1);
        seq_t digit = rem / fact;
        rem = rem % fact;
        sum += digit * next_fact;
        fact = fact / (seq_t)cur;
    }
    return sum;
}

static seq_t *A265905_cache = NULL;
static size_t A265905_len = 0;
static size_t A265905_cap = 0;

static void A265905_ensure(idx_t n)
{
    if (n == 0) return;
    if (A265905_len == 0) {
        A265905_cap = 16;
        A265905_cache = mem_new(seq_t, A265905_cap);
        A265905_cache[0] = (seq_t)1;
        A265905_len = 1;
    }
    if (A265905_cap < (size_t)n) {
        size_t new_cap = A265905_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        A265905_cache = mem_resize(seq_t, A265905_cache, new_cap);
        A265905_cap = new_cap;
    }
    for (size_t i = A265905_len; i < (size_t)n; ++i) {
        seq_t prev = A265905_cache[i - 1];
        A265905_cache[i] = prev + A153880_h(prev);
    }
    if ((size_t)n > A265905_len) A265905_len = (size_t)n;
}

static seq_t A265905_a(idx_t n)
{
    A265905_ensure(n);
    return A265905_cache[(size_t)(n - 1)];
}

seq_t a(idx_t n)
{
    seq_t x1 = A265905_a(n);
    seq_t x2 = A265905_a(n + 1);
    return A153880_h(x2) - A153880_h(x1);
}
