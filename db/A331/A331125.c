/*
 * A331125  Numbers k such that there is no prime p between k and (9/8)k,
 *          exclusive.
 * https://oeis.org/A331125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: a(n) = the n‑th integer k such that no prime lies strictly between
   k and (9/8)·k (exclusive).  The sequence is finite; the last term is 47. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* static storage for the computed terms */
static size_t _a_len = 0;
static seq_t _terms[47];

/* Build the table of terms on first use. */
static void _a331125_build(void)
{
    if (_a_len != 0) return;            /* already built */

    for (idx_t k = 1; k <= 47; ++k) {
        idx_t upper = (9 * k) / 8;                        /* floor(9k/8)   */
        if ((9 * k) % 8 == 0) upper -= 1;                 /* drop the upper
                                                             bound if 9k/8 is
                                                             an integer        */
        bool has_prime = false;
        if (upper >= k + 1) {
            for (idx_t x = k + 1; x <= upper; ++x) {
                if (is_prime(x)) { has_prime = true; break; }
            }
        }
        if (!has_prime) { _terms[_a_len++] = (seq_t)k; }
    }
}

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, just return 0 */
    _a331125_build();
    if (n > _a_len) return (seq_t)0;   /* no such term beyond the last one */
    return _terms[n - 1];
}
