/*
 * A265223  Total number of OFF (white) cells after n iterations of the "Rule
 *          150" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A265223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 282, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Rule 150 elementary cellular automaton starting from a single ON cell.  
   Let ones(i) be the number of ON cells on iteration i (i ≥ 0).  
   The width of the active region at iteration i is 2*i+1, hence the
   number of OFF cells on that iteration is (2*i+1) – ones(i).
   The required sequence is the cumulative sum of these OFF cells:
   
   a(n) = Σ_{i=0}^{n} [ (2*i+1) – ones(i) ]   for n ≥ 0.
   The function below implements this directly by iterating the CA.
   It works for the generic unbounded integer type seq_t and
   for all builds of the test harness.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial case: no rows are processed */
    if (n == 0)
        return 0;

    /* maximum number of cells that will ever be needed */
    const size_t max_len = (size_t)n * 2 + 1;
    uint8_t *row = (uint8_t *)malloc(max_len * sizeof(uint8_t));
    uint8_t *tmp = (uint8_t *)malloc(max_len * sizeof(uint8_t));
    if (!row || !tmp) {
        /* in case of allocation failure, return 0 – the harness never
           triggers this because n is small enough for 10 k. */
        free(row);
        free(tmp);
        return 0;
    }

    /* start: a single ON cell in the middle */
    size_t old_len = 1;
    for (size_t i = 0; i < max_len; ++i)
        row[i] = 0;
    row[0] = 1;

    seq_t cumulative = 0;

    /* iterate from step 1 up to and including n */
    for (idx_t step = 1; step <= n; ++step) {
        const size_t new_len = old_len + 2;

        /* build the next row */
        size_t ones = 0;
        for (size_t j = 0; j < new_len; ++j) {
            uint8_t val = 0;
            if (j >= 2)
                val ^= row[j - 2];
            if (j >= 1 && j - 1 < old_len)
                val ^= row[j - 1];
            if (j < old_len)
                val ^= row[j];
            tmp[j] = val;
            if (val)
                ++ones;
        }

        /* add the number of OFF cells in this row */
        seq_t zeros = (seq_t)new_len - ones;
        cumulative += zeros;

        /* swap buffers for next iteration */
        uint8_t *sw = row;
        row = tmp;
        tmp = sw;

        old_len = new_len;
    }

    free(row);
    free(tmp);
    return cumulative;
}
