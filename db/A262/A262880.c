/*
 * A262880  Number of ordered ways to write n as w*(w+1)/2 + x^3 + y^3 + 2*z^3
 *          with w > 0, 0 <= x <= y and z >= 0.
 * https://oeis.org/A262880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A262880: a(n) = number of ordered solutions to
   n = w*(w+1)/2 + x^3 + y^3 + 2*z^3
   with w > 0, 0 <= x <= y, z >= 0.
*/
#include <stdint.h>

static const idx_t cubes[22] = {
    0ULL, 1ULL, 8ULL, 27ULL, 64ULL, 125ULL, 216ULL, 343ULL,
    512ULL, 729ULL, 1000ULL, 1331ULL, 1728ULL, 2197ULL,
    2744ULL, 3375ULL, 4096ULL, 4913ULL, 5832ULL,
    6859ULL, 8000ULL, 9261ULL
};

static int cube_root[10001];
static void init_cube_root(void)
{
    static int init_done = 0;
    if (init_done) return;
    for (int i = 0; i <= 10000; ++i) cube_root[i] = -1;
    for (idx_t i = 0; i < 22; ++i) cube_root[cubes[i]] = (int)i;
    init_done = 1;
}

/* integer cube root: floor(v^(1/3)) for 0 <= v <= 10000 */
static inline idx_t int_cuberoot(idx_t v)
{
    idx_t r = 0;
    while ((r + 1ULL) * (r + 1ULL) * (r + 1ULL) <= v) ++r;
    return r;
}

seq_t a(idx_t n)
{
    init_cube_root();
    seq_t count = 0;
    for (idx_t w = 1; ; ++w) {
        idx_t T = w * (w + 1) / 2;
        if (T > n) break;
        idx_t R = n - T;
        idx_t zmax = int_cuberoot(R / 2);
        for (idx_t z = 0; z <= zmax; ++z) {
            idx_t rem1 = R - 2 * z * z * z;
            if (rem1 < 0) continue;
            idx_t xmax = int_cuberoot(rem1);
            for (idx_t x = 0; x <= xmax; ++x) {
                idx_t xcube = cubes[x];
                idx_t y_cube = rem1 - xcube;
                if (y_cube < xcube) continue;          /* enforce y >= x */
                int y_root = cube_root[y_cube];
                if (y_root >= 0 && (idx_t)y_root * y_root * y_root == y_cube) {
                    ++count;
                }
            }
        }
    }
    return count;
}
