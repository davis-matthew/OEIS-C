/*
 * A309414  Number of squaring iterations necessary to achieve the minimal
 *          residue class (mod n).
 * https://oeis.org/A309414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum number of iterations needed to reach the fixed
 *            set under repeated squaring modulo n; equivalently the maximum
 *            distance from any residue to a cycle in the functional graph x →
 *            x² mod n
 * Range:     u8 n <= 555, u16 n <= 9671, u32 n <= 9671, u64 n <= 9671,
 *            u128 n <= 9671, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    size_t N = (size_t)n;
    int *depth = (int*)malloc(N * sizeof(int));
    int *visit = (int*)calloc(N, sizeof(int));
    if (!depth || !visit) { free(depth); free(visit); return (seq_t)-1; }

    for (size_t i = 0; i < N; ++i) depth[i] = -1;

    int max_depth = 0;
    int *path = (int*)malloc(N * sizeof(int));
    if (!path) { free(depth); free(visit); return (seq_t)-1; }

    for (idx_t start = 0; start < n; ++start) {
        if (depth[(size_t)start] != -1) continue;

        size_t sz = 0;
        idx_t cur = start;

        while (1) {
            if (depth[(size_t)cur] != -1) {
                int d = depth[(size_t)cur];
                for (int j = (int)sz - 1; j >= 0; --j) {
                    d++;
                    depth[(size_t)path[j]] = d;
                    if (d > max_depth) max_depth = d;
                }
                break;
            }
            if (visit[(size_t)cur] != 0) {
                size_t cycle_start = (size_t)(visit[(size_t)cur]) - 1;
                for (size_t j = cycle_start; j < sz; ++j) depth[(size_t)path[j]] = 0;
                int d = 0;
                for (int j = (int)cycle_start - 1; j >= 0; --j) {
                    d++;
                    depth[(size_t)path[j]] = d;
                    if (d > max_depth) max_depth = d;
                }
                break;
            }
            visit[(size_t)cur] = (int)sz + 1; /* store index+1 */
            path[sz++] = (int)cur;
            cur = (idx_t)(((seq_t)cur * (seq_t)cur) % n);
        }

        for (size_t j = 0; j < sz; ++j) visit[(size_t)path[j]] = 0;
    }

    free(depth);
    free(visit);
    free(path);

    return (seq_t)max_depth;
}
