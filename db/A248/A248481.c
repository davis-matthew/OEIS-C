/*
 * A248481  y-values in the solutions to x^2 + 999 = y^3.
 * https://oeis.org/A248481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the nth integer y for which there exists an integer x
 *            such that x^2 + 999 = y^3. The sequence is the list of y-values
 *            in the integer solutions of the Diophantine equation x^2 + 999 =
 *            y^3, ordered by increasing y. The function builds the solutions
 *            on demand and memoises them across calls
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) returns ⌊√x⌋ */

seq_t a(idx_t n) {
    /* Fail fast for n < 1 */
    if (n == 0) return 0;

    /* Static storage for memoised solutions */
    static seq_t *solutions = NULL;   /* array of found y-values */
    static size_t sol_len = 0;        /* number of solutions found */
    static size_t sol_cap = 0;        /* current allocation capacity */
    static idx_t next_y = 1;          /* next y to test */

    /* Allocate initial cache on first call */
    if (sol_cap == 0) {
        sol_cap = 16;
        solutions = (seq_t*)malloc(sol_cap * sizeof(seq_t));
        if (solutions == NULL) return 0;  /* allocation failure */
    }

    /* Generate solutions until we have at least n of them */
    while (sol_len < n) {
        /* y³ computation */
        seq_t ysq3 = (seq_t)next_y * next_y * next_y;

        /* Only consider y where y³ ≥ 999 to avoid unsigned underflow */
        if (ysq3 >= (seq_t)999) {
            seq_t v = ysq3 - (seq_t)999;          /* y³ - 999 */
            seq_t r = floor_sqrt(v);             /* ⌊√v⌋ */
            if (r * r == v) {                    /* v is a perfect square */
                /* Grow solutions array if needed */
                if (sol_len >= sol_cap) {
                    size_t new_cap = sol_cap * 2;
                    seq_t *tmp = (seq_t*)realloc(solutions, new_cap * sizeof(seq_t));
                    if (tmp == NULL) {
                        /* Allocation failure – abort further work */
                        free(solutions);
                        solutions = NULL;
                        sol_cap = sol_len = 0;
                        return 0;
                    }
                    solutions = tmp;
                    sol_cap = new_cap;
                }
                /* Store the y-value */
                solutions[sol_len++] = (seq_t)next_y;
            }
        }
        next_y++;   /* move to the next candidate y */
    }

    /* Return the requested nth solution (1‑based indexing) */
    return solutions[n - 1];
}
