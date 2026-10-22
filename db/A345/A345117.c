/*
 * A345117  a(n) is the index (in Z/nZ) of the first already visited element
 *          in the process of moving around Z/nZ, starting at 0 with stride 1
 *          and increasing stride by 1 after each step.
 * https://oeis.org/A345117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index of first already visited element in the process of
 *            moving around Z/nZ starting at 0 with stride 1 and increasing
 *            stride by 1 after each move. The sequence can be generated
 *            directly by simulation in O(n) time and O(n) memory
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The process always terminates in at most n steps, because there
       are only n residues.  Use a dynamic bit array to mark visited
       positions. */
    unsigned char *visited = (unsigned char *)malloc((size_t)n);
    if (!visited)           /* unlikely, but keep the function well‑defined */
        return (seq_t)0;

    memset(visited, 0, (size_t)n);
    visited[0] = 1;

    idx_t pos    = 0;  /* current position */
    idx_t stride = 1;  /* current stride length */

    for (;;)
    {
        pos += stride;
        pos %= n;                    /* wrap around Z/nZ */
        if (visited[(size_t)pos])    /* already visited? */
        {
            free(visited);
            return (seq_t)pos;
        }
        visited[(size_t)pos] = 1;     /* mark as visited */
        ++stride;                     /* increase stride for next step */
    }
}
