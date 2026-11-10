/*
 * A352368  Alternating row sums of A352366.
 * https://oeis.org/A352368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * A352366(n,k); equivalently a(0) =
 *            1, a(n) = -Sum_{i=1..n} binomial(n-1,i-1) * Catalan(i-1) *
 *            a(n-i) for n >= 1
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 15, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-10)
 * Helpers:   binomial, catalan, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"
#include "../utils/mem.h"

static seq_t *A352368_B = NULL;
static idx_t A352368_cnt = 0;

seq_t a(idx_t n)
{
    if (A352368_cnt <= n) {
        A352368_B = mem_resize(seq_t, A352368_B, (size_t)(n + 1));
        for (idx_t m = A352368_cnt; m <= n; ++m) {
            if (m == 0) {
                A352368_B[0] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i <= m; ++i) {
                sum += binomial(m - 1, i - 1) * catalan(i - 1) * A352368_B[m - i];
            }
            A352368_B[m] = -sum;
        }
        A352368_cnt = n + 1;
    }
    return A352368_B[n];
}
