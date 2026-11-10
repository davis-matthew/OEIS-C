/*
 * A376047  Complement of A092433.
 * https://oeis.org/A376047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer not divisible by 7 and with no
 *            digit 7 in base 10 (search by sieve-like scan)
 * Range:     u8 n <= 182, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9881 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A376047_ok(seq_t x)
{
    if (x % 7 == 0) return false;
    while (x > 0) {
        if (x % 10 == 7) return false;
        x /= 10;
    }
    return true;
}

static seq_t *A376047_cache = NULL;
static idx_t A376047_len = 0;
static idx_t A376047_cap = 0;
static seq_t A376047_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A376047_len < n) {
        while (!A376047_ok(A376047_next)) A376047_next++;
        if (A376047_len >= A376047_cap) {
            idx_t nc = A376047_cap ? A376047_cap * 2 : 64;
            A376047_cache = mem_resize(seq_t, A376047_cache, nc);
            A376047_cap = nc;
        }
        A376047_cache[A376047_len++] = A376047_next;
        A376047_next++;
    }
    return A376047_cache[n - 1];
}
