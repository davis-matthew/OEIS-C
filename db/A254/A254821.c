/*
 * A254821  Number of length n 1..(2+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 32, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A254821: Count of length‑n arrays over {1,2,3} whose every leading partial sum
   is divisible by 2, 3, or 5.  The DP uses residues modulo 30 (lcm(2,3,5)). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Residues that are acceptable (divisible by 2, 3, or 5). */
    static bool ok[30];
    static bool ok_init = false;
    if (!ok_init) {
        for (idx_t r = 0; r < 30; ++r)
            ok[r] = (r % 2 == 0) || (r % 3 == 0) || (r % 5 == 0);
        ok_init = true;
    }

    /* DP arrays for current and next step.  Only residues that satisfy the
       divisibility condition will be kept. */
    static seq_t dp_cur[30];
    static seq_t dp_next[30];

    /* Initialize for length 0 (empty prefix). */
    for (idx_t i = 0; i < 30; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1;          /* sum 0 is acceptable */

    /* Iterate over each position. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero next array. */
        for (idx_t i = 0; i < 30; ++i) dp_next[i] = 0;

        /* Transition from each residue. */
        for (idx_t r = 0; r < 30; ++r) {
            seq_t val = dp_cur[r];
            if (!val) continue;

            /* Add 1, 2, or 3 and check the resulting residue. */
            idx_t r1 = (r + 1) % 30;
            if (ok[r1]) dp_next[r1] += val;

            idx_t r2 = (r + 2) % 30;
            if (ok[r2]) dp_next[r2] += val;

            idx_t r3 = (r + 3) % 30;
            if (ok[r3]) dp_next[r3] += val;
        }

        /* Swap dp_cur and dp_next for the next iteration. */
        for (idx_t i = 0; i < 30; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all acceptable residues after n steps. */
    seq_t result = 0;
    for (idx_t i = 0; i < 30; ++i) result += dp_cur[i];
    return result;
}
