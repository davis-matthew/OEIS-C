/*
 * A376589  Points of nonzero curvature in the sequence of non-perfect-powers
 *          (A007916).
 * https://oeis.org/A376589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computation: returns the n‑th index k for which the second difference
   of the sequence of non‑perfect‑powers (A007916) is non‑zero.  The
   function generates the sequence of non‑perfect‑powers on the fly
   and detects changes in consecutive differences. */

static bool is_perfect_power(uint64_t x)
{
    if (x == 1) return true;      /* 1 = 1^k is a perfect power   */
    if (x < 4) return false;      /* 2,3 are not perfect powers   */

    for (uint64_t base = 2; base * base <= x; ++base) {
        uint64_t pow = base * base;
        while (pow <= x) {
            if (pow == x) return true;
            if (pow > x / base) break;   /* avoid overflow next multiply */
            pow *= base;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* defined only for n≥1 */

    uint64_t diff_prev = 0;              /* previous difference d_{k-1}      */
    uint64_t diff_index = 0;             /* current difference index k       */
    uint64_t candidate = 2;              /* current integer to test         */
    uint64_t prev_value = 0;             /* previous non‑perfect‑power value*/
    bool first = true;                   /* flag for first number          */
    idx_t found = 0;                     /* how many curvature points found */

    while (found < n) {
        if (first) {
            prev_value = candidate;      /* x_1 = 2                         */
            first = false;
            ++candidate;
            continue;
        }

        if (is_perfect_power(candidate)) {   /* skip perfect powers          */
            ++candidate;
            continue;
        }

        uint64_t curr_value = candidate;
        ++candidate;

        ++diff_index;                       /* new difference d_k            */
        uint64_t diff_curr = curr_value - prev_value;

        if (diff_index == 1) {
            diff_prev = diff_curr;           /* no change detection yet       */
        } else {
            if (diff_curr != diff_prev) {    /* curvature point found         */
                ++found;
                if (found == n)
                    return (seq_t)(diff_index - 1);  /* index k where change occurs */
            }
            diff_prev = diff_curr;
        }
        prev_value = curr_value;
    }

    /* fall‑through, should not happen */
    return (seq_t)(diff_index - 1);
}
