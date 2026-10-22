/*
 * A349991  For any n >= 0, consider a sandpile model on the infinite
 *          hexagonal lattice starting with n grains at the origin, the other
 *          sites being empty; a(n) gives the number of nonempty sites after
 *          stabilization of this sandpile model.
 * https://oeis.org/A349991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by simulating the Abelian sandpile on the infinite
 *            hexagonal lattice with threshold 6. For each n, the function
 *            initializes a finite grid large enough to contain the
 *            stabilisation of n grains (n ≤ 10 000), applies topplings until
 *            all sites are stable, and counts the non‑empty sites....
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n < 6)  return 1;

    /* Grid size comfortably covers radius ≈ 30 for n up to 10 000 */
    const int SIZE   = 151;          /* even > 2 * expected radius */
    const int OFFSET = SIZE / 2;     /* center of the array */

    seq_t grid[SIZE * SIZE];
    for (int i = 0; i < SIZE * SIZE; ++i) grid[i] = 0;

    int ox = OFFSET, oy = OFFSET;          /* origin coordinates */
    grid[ox * SIZE + oy] = (seq_t)n;

    const int dirs[6][2] = {
        { 1,  0}, {-1,  0},
        { 0,  1}, { 0, -1},
        { 1, -1}, {-1,  1}
    };

    bool changed;
    do
    {
        changed = false;
        for (int i = 0; i < SIZE; ++i)
        {
            for (int j = 0; j < SIZE; ++j)
            {
                seq_t val = grid[i * SIZE + j];
                if (val >= 6)
                {
                    seq_t topple = val / 6;
                    grid[i * SIZE + j] -= topple * 6;
                    for (int d = 0; d < 6; ++d)
                    {
                        int ni = i + dirs[d][0];
                        int nj = j + dirs[d][1];
                        if (ni >= 0 && ni < SIZE && nj >= 0 && nj < SIZE)
                            grid[ni * SIZE + nj] += topple;
                    }
                    changed = true;
                }
            }
        }
    } while (changed);

    seq_t count = 0;
    for (int i = 0; i < SIZE * SIZE; ++i)
        if (grid[i] > 0) ++count;

    return count;
}
