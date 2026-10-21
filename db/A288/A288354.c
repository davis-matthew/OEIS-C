/*
 * A288354  Strings of 4 digits from 1...9, such that no formula using the
 *          single digits in the given order exists that evaluates to 0.
 * https://oeis.org/A288354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   Sequence A288354 computed by brute force enumeration of all *
 *            4‑digit strings from 1…9, keeping those that cannot be evaluated
 *            to * 0 by any arithmetic expression using the digits in the
 *            given order
 * Range:     u8 none, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   apply_op, has_zero_expression, llgcd, normalize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/apply_op.h"
#include "../utils/has_zero_expression.h"
#include "../utils/llgcd.h"
#include "../utils/normalize.h"

/* --- Main sequence function ------------------------------------------- */
static seq_t seq_cache[1500];
static bool seq_ready = false;
static idx_t seq_len = 0;

seq_t a(idx_t n)
{
    if (!seq_ready) {
        static const int MAX_COUNT = 1500;
        int d[4];
        sepaml:
        for (int d1 = 1; d1 <= 9; ++d1) {
            for (int d2 = 1; d2 <= 9; ++d2) {
                for (int d3 = 1; d3 <= 9; ++d3) {
                    for (int d4 = 1; d4 <= 9; ++d4) {
                        d[0] = d1; d[1] = d2; d[2] = d3; d[3] = d4;
                        if (!has_zero_expression(d)) {
                            seq_t val = (seq_t)(d1 * 1000 + d2 * 100 + d3 * 10 + d4);
                            seq_cache[seq_len++] = val;
                        }
                    }
                }
            }
        }
        seq_ready = true;
    }

    if (n == 0 || n > seq_len) return 0;   /* out of bounds -> 0 */
    return seq_cache[n - 1];
}
