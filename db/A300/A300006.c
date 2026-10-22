/*
 * A300006  Matrices of the 2 X 2 sandpile group, with matrix [a,b;c,d]
 *          encoded as concat(a,b,c,d), leading 0 omitted.
 * https://oeis.org/A300006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 192, u32 n <= 192, u64 n <= 192,
 *            u128 n <= 192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300006 – 2×2 sandpile group matrices encoded without leading zeros.
   The function precomputes all 192 recurrent configurations (0‑3 tallies)
   and returns the (n‑1)-th element (1‑based index).  The configurations
   are sorted lexicographically, which is equivalent to the numeric
   order of their decimal concatenations.  */

#include <stdlib.h>
#include <stdbool.h>

static const int nb1[4] = {1, 0, 0, 1};   /* first neighbor of each cell   */
static const int nb2[4] = {2, 3, 3, 2};   /* second neighbor of each cell  */

seq_t a(idx_t n) {
    /* Lazy static cache of the sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Build cache on first call */
    if (cache == NULL) {
        /* temporary storage is safe because count <= 256 */
        seq_t tmp[256];
        int  cnt = 0;

        for (int a0 = 0; a0 < 4; ++a0)
            for (int b0 = 0; b0 < 4; ++b0)
                for (int c0 = 0; c0 < 4; ++c0)
                    for (int d0 = 0; d0 < 4; ++d0) {
                        int cells[4] = {a0 + 2, b0 + 2, c0 + 2, d0 + 2};

                        /* topple until stable */
                        while (1) {
                            bool any = false;
                            for (int i = 0; i < 4; ++i) {
                                if (cells[i] > 3) {
                                    cells[i] -= 4;
                                    cells[nb1[i]]++;
                                    cells[nb2[i]]++;
                                    any = true;
                                }
                            }
                            if (!any) break;
                        }

                        /* check invariance under addition of E2 */
                        if (cells[0] == a0 && cells[1] == b0 &&
                            cells[2] == c0 && cells[3] == d0) {
                            int rep = a0 * 1000 + b0 * 100 + c0 * 10 + d0;
                            tmp[cnt++] = (seq_t)rep;
                        }
                    }

        /* Allocate final cache */
        cache_len = cnt; /* should be 192 */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = tmp[i];
    }

    /* Return the requested element if within bounds */
    if (n == 0 || n - 1 >= cache_len) return (seq_t)0;
    return cache[n - 1];
}
