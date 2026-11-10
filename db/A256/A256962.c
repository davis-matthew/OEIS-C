/*
 * A256962  Numbers n such that A005210(n) = A005210(n-1).
 * https://oeis.org/A256962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k such that A005210(k) = A005210(k-1), where
 *            A005210(k) = |A005210(k-1) + 2*A005210(k-2) - k|, A005210(1) =
 *            A005210(2) = 1; computed by a linear search with a resumable
 *            state
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t A256962_terms[16];
static int A256962_count = 0;
static bool A256962_init = false;
static idx_t A256962_i = 2;
static seq_t A256962_px, A256962_py;

seq_t a(idx_t n) {
    if (n < 1 || n > 8) return 0;
    if (!A256962_init) {
        A256962_init = true;
        A256962_px = 1;
        A256962_py = 1;
        A256962_i = 2;
        A256962_terms[A256962_count++] = 2; /* A005210(2) == A005210(1) */
    }
    while ((idx_t)A256962_count < n) {
        A256962_i++;
        seq_t k = (seq_t)A256962_i;
        seq_t s = A256962_py + 2 * A256962_px;
        seq_t z = (s >= k) ? (s - k) : (k - s);
        if (z == A256962_py) {
            A256962_terms[A256962_count++] = A256962_i;
        }
        A256962_px = A256962_py;
        A256962_py = z;
    }
    return (seq_t)A256962_terms[n - 1];
}
