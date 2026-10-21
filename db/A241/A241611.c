/*
 * A241611  Number of length n+2 0..5 arrays with no consecutive three
 *          elements summing to more than 5.
 * https://oeis.org/A241611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+2 0..5 arrays with no consecutive *
 *            three elements summing to more than 5. The value a(n) is *
 *            computed by dynamic programming on the last two digits
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n<1 */

    /* We use a 6×6 table because each digit can be 0…5. */
    const int K = 6;
    seq_t cur[K][K];
    seq_t nxt[K][K];

    /* Initialisation : length = 2, every pair is allowed. */
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            cur[i][j] = 1;

    /* Extend the sequence until length = n+2. */
    for (idx_t len = 2; len < n + 2; ++len) {
        /* Clear the next table. */
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                nxt[i][j] = 0;

        /* Transition: (x,y) → (y,z) if x+y+z ≤ 5. */
        for (int x = 0; x < K; ++x) {
            for (int y = 0; y < K; ++y) {
                seq_t v = cur[x][y];
                if (!v) continue;
                for (int z = 0; z < K; ++z) {
                    if ((seq_t)(x + y + z) <= 5)
                        nxt[y][z] += v;
                }
            }
        }

        /* Prepare for next iteration. */
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* Sum all ending states for length n+2. */
    seq_t total = 0;
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            total += cur[i][j];

    return total;
}
