/*
 * A366507  Numbers k such that the sum of the digits of k times the square of
 *          the sum of the digits cubed of k equals k.
 * https://oeis.org/A366507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th integer k such that digit_sum(k) *
 *            (digit_cube_sum(k))^2 == k (OEIS A366507). This implementation
 *            generates all solutions by enumerating possible digit sums s and
 *            cube‐sums c and checking the property. It is memoised, so it
 *            handles calls in any order. For n beyond the number of so...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   cube_sum_of_digits, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* includes needed for the util helpers */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/cube_sum_of_digits.h"

/* forward declaration of helper to compute the cube sum of a number */
static seq_t cube_sum_of_digits(seq_t x);

/* memoised solution list */
static seq_t solutions[64];
static size_t sol_cnt = 0;
static bool initialized = false;

/* initialise mapping of all solutions once */
static void init_solutions(void)
{
    if (initialized) return;
    initialized = true;
    sol_cnt = 0;

    /* Upper bounds that safely cover all known solutions.
       The largest known c is 2231, so 10000 is ample. */
    const unsigned max_s = 200;   /* maximal possible digit sum */
    const unsigned max_c = 10000; /* maximal possible digit cube sum */

    for (unsigned s = 1; s <= max_s; ++s) {
        for (unsigned c = 1; c <= max_c; ++c) {
            seq_t k = (seq_t)s * (seq_t)c * (seq_t)c;  /* k = s * c^2 */

            /* verify digit sum matches s */
            seq_t ds = digit_sum(k);
            if (ds != (seq_t)s) continue;

            /* compute cube sum of digits and compare to c */
            seq_t cs = cube_sum_of_digits(k);
            if (cs != (seq_t)c) continue;

            /* we have a valid solution; ensure uniqueness */
            bool dup = false;
            for (size_t i = 0; i < sol_cnt; ++i) {
                if (solutions[i] == k) { dup = true; break; }
            }
            if (!dup) {
                if (sol_cnt < sizeof(solutions)/sizeof(solutions[0])) {
                    solutions[sol_cnt++] = k;
                }
            }
        }
    }

    /* sort solutions in increasing order */
    for (size_t i = 0; i < sol_cnt; ++i) {
        for (size_t j = i + 1; j < sol_cnt; ++j) {
            if (solutions[i] > solutions[j]) {
                seq_t tmp = solutions[i];
                solutions[i] = solutions[j];
                solutions[j] = tmp;
            }
        }
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    init_solutions();
    if ((idx_t)sol_cnt < n) return 0;
    return solutions[n - 1];
}
