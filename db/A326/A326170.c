/*
 * A326170  Let z be a sequence of distinct Gaussian integers such that z(1) =
 *          0, z(2) = 2+i (where i denotes the imaginary unit), for n > 1,
 *          z(n+1) the Gaussian integer with least norm at one knight move
 *          from z(n) (in case of a tie, choose the value such that
 *          Im(z(n+1)/z(n))>0); a(n) is the real part of z(n).
 * https://oeis.org/A326170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini
 * Formula:   not stated
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326170: Knight walk on Gaussian integers.
   For n ≥ 1 let z(1)=0, z(2)=2+i and for n>1 let
   z(n+1) be the unvisited knight‑move neighbor of z(n) with
   smallest norm.  If several neighbors have the same
   minimal norm, choose the one with Im(z(n+1)/z(n))>0.
   a(n) = Re(z(n)).                           */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define OFF 400            /* to offset coordinates into array         */
#define SZ  801            /* array size = OFF*2+1; enough for walk    */

/* Return the real part of z(n). */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 1) return 0;
    if (n == 2) return 2;

    /* visited map: 0 = not visited, 1 = visited */
    static unsigned char visited[SZ][SZ];
    memset(visited, 0, sizeof(visited));

    /* start positions */
    const seq_t start_x = 2;
    const seq_t start_y = 1;
    visited[OFF + 0][OFF + 0] = 1;                 /* z(1) = 0+0i   */
    visited[OFF + (int)((long long)start_x)][OFF + (int)((long long)start_y)] = 1;
    seq_t x = start_x, y = start_y;               /* current position z(i) */

    /* knight move deltas */
    const int dx[8] = { 1,  1, -1, -1,  2,  2, -2, -2 };
    const int dy[8] = { 2, -2,  2, -2,  1, -1,  1, -1 };

    for (idx_t i = 2; i < n; ++i)                  /* need n-1 moves   */
    {
        seq_t best_x = 0, best_y = 0;
        seq_t best_norm = 0;
        seq_t best_cross = 0;
        bool found = false;

        for (int k = 0; k < 8; ++k)
        {
            seq_t nx = x + (seq_t)dx[k];
            seq_t ny = y + (seq_t)dy[k];

            int ix = (int)(((long long)nx) + OFF);
            int iy = (int)(((long long)ny) + OFF);
            if (ix < 0 || ix >= SZ || iy < 0 || iy >= SZ)
                continue;                           /* safety */

            if (visited[ix][iy])
                continue;                           /* already visited */

            seq_t norm = nx * nx + ny * ny;

            if (!found || norm < best_norm)
            {
                found = true;
                best_norm = norm;
                best_x = nx;
                best_y = ny;
                best_cross = ny * x - nx * y;        /* Im(z_{next}/z) */
            }
            else if (norm == best_norm)
            {
                seq_t cross = ny * x - nx * y;
                if (cross > 0 && best_cross <= 0)    /* choose Im>0   */
                {
                    best_x = nx;
                    best_y = ny;
                    best_cross = cross;
                }
            }
        }

        /* move to the chosen neighbour */
        visited[OFF + (int)(((long long)best_x))][OFF + (int)(((long long)best_y))] = 1;
        x = best_x;
        y = best_y;
    }

    return x;                                       /* real part of z(n) */
}
