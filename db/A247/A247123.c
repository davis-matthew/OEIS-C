/*
 * A247123  Palindromes obtained after one iteration of Reverse and Add
 *          applied to the terms of A015976.
 * https://oeis.org/A247123
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = m + reverse(m) where m = A015976(n-1), the (n-1)-th
 *            integer m with m + reverse(m) palindromic (found by search)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 50 (2026-11-07)
 * Helpers:   is_palindrome_base, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_palindrome_base.h"
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

static seq_t *A247123_cache = NULL;
static size_t A247123_cap = 0;
static size_t A247123_len = 0;
static seq_t A247123_checked = 0;

seq_t a(idx_t n)
{
    size_t need = (size_t)(n - 1);
    while (A247123_len < need) {
        A247123_checked++;
        seq_t rev = reverse_digits(A247123_checked);
        seq_t sum = A247123_checked + rev;
        if (is_palindrome_base(sum, 10)) {
            if (A247123_len >= A247123_cap) {
                size_t nc = A247123_cap ? A247123_cap * 2 : 16;
                seq_t *p = mem_resize(seq_t, A247123_cache, nc);
                if (!p) abort();
                A247123_cache = p;
                A247123_cap = nc;
            }
            A247123_cache[A247123_len++] = sum;
        }
    }
    return A247123_cache[need - 1];
}
