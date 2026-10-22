/*
 * A320170  Every pair of consecutive numbers sums to a palindrome, starting
 *          with a(0)=1; a(1)=10, and taking a(n) = smallest number > a(n-2).
 * https://oeis.org/A320170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   grow_seq, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A320170
 * a(0) = 1, a(1) = 10
 * For n ≥ 2: a(n) is the smallest integer > a(n−2) such that
 * a(n) + a(n−1) is a palindrome in base 10. */

#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    if ((size_t)n >= len) {
        size_t need = (size_t)n + 1;
        grow_seq(&cache, &cap, need);

        for (idx_t i = (idx_t)len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)10;
            } else {
                seq_t cand = cache[i - 2] + (seq_t)1;
                while (!is_palindrome(cache[i - 1] + cand, (idx_t)10)) {
                    cand += (seq_t)1;
                }
                cache[i] = cand;
            }
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
