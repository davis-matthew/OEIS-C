/*
 * A352226  Consider a 2D sandpile model where each site with 2 or more
 *          grains, say at location (x, y), topples and transfers one grain of
 *          sand to the sites at locations (x+1, y) and (x, y+1). Let S(n) be
 *          the configuration after stabilization of a configuration with n
 *          grains at the origin. a(n) = Max_{ (x,y)...
 * https://oeis.org/A352226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{(x,y) in S(n)} (x + y) (S(n) is the final stable *
 *            configuration of the directed sandpile with one grain at the
 *            origin * before toppling). This implementation simulates the
 *            process using * a bounded array and returns the maximum
 *            Manhattan distance of an * occupied site
 * Range:     u8 n <= 2447, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   push
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/push.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Size of the working grid.  The support of the final configuration
     * grows roughly like 2 * sqrt(n).  A safety factor of 4 provides ample room
     * for n up to at least 10,000. */
    uint32_t M = (uint32_t)(4.0 * sqrt((double)n) + 10.0);
    if (M < 10) M = 10;

    /* Allocate buffers.  Use calloc so that the grid is initialized to 0. */
    uint32_t *grid = (uint32_t *)calloc((size_t)M * M, sizeof(uint32_t));
    uint8_t  *inq  = (uint8_t  *)calloc((size_t)M * M, sizeof(uint8_t));
    uint16_t *qx   = (uint16_t *)malloc((size_t)M * M * sizeof(uint16_t));
    uint16_t *qy   = (uint16_t *)malloc((size_t)M * M * sizeof(uint16_t));
    if (!grid || !inq || !qx || !qy) {
        free(grid);
        free(inq);
        free(qx);
        free(qy);
        return (seq_t)0;   /* allocation failure */
    }

    uint32_t head = 0, tail = 0;
    /* Place all n grains at the origin. */
    grid[0] = (uint32_t)n;
    if (grid[0] >= 2) push(0, 0, M, inq, qx, qy, &tail);

    /* Process the queue of unstable sites. */
    while (head < tail) {
        uint32_t x = qx[head];
        uint32_t y = qy[head];
        head++;
        size_t idx = (size_t)x * M + y;
        inq[idx] = 0;

        uint32_t val = grid[idx];
        if (val < 2) continue;        /* already stable by the time we process it */

        uint32_t t = val >> 1;        /* number of whole topplings */
        grid[idx] = val & 1;          /* remainder 0 or 1 */

        /* Transfer to the east neighbor (x+1, y). */
        size_t idx_east = (size_t)(x + 1) * M + y;
        grid[idx_east] += t;
        if (grid[idx_east] >= 2) push(x + 1, y, M, inq, qx, qy, &tail);

        /* Transfer to the north neighbor (x, y+1). */
        size_t idx_north = (size_t)x * M + (y + 1);
        grid[idx_north] += t;
        if (grid[idx_north] >= 2) push(x, y + 1, M, inq, qx, qy, &tail);
    }

    /* Compute the maximum x + y among occupied cells. */
    uint32_t max_sum = 0;
    for (size_t i = 0; i < (size_t)M * M; ++i) {
        if (grid[i]) {
            uint32_t x = i / M;
            uint32_t y = i % M;
            uint32_t sum = x + y;
            if (sum > max_sum) max_sum = sum;
        }
    }

    free(grid);
    free(inq);
    free(qx);
    free(qy);

    return (seq_t)max_sum;
}
