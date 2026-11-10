/*
 * A247900  First differences of A247681.
 * https://oeis.org/A247900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A247681(n+1) - A247681(n), where A247681 lists the odd
 *            nonprimes == 1 (mod 9), i.e. nonprimes of the form 18k+1 (found
 *            by search with trial division)
 * Range:     u8 n <= 6, u16 n <= 7430, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 96 terms through n = 9588 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A247900_cache = NULL;
static size_t A247900_cap = 0;
static size_t A247900_len = 0;
static seq_t A247900_next = 1;   /* next candidate 18k+1 */

static bool A247900_isprime(seq_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (seq_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) return false;
    }
    return true;
}

static void A247900_fill(size_t need)
{
    while (A247900_len < need) {
        seq_t c = A247900_next;
        A247900_next += 18;
        if (c == 1 || !A247900_isprime(c)) {
            if (A247900_len == A247900_cap) {
                size_t nc = A247900_cap ? A247900_cap * 2 : 64;
                seq_t *t = mem_resize(seq_t, A247900_cache, nc);
                A247900_cache = t;
                A247900_cap = nc;
            }
            A247900_cache[A247900_len++] = c;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A247900_fill((size_t)n + 1);
    return A247900_cache[n] - A247900_cache[n - 1];
}
