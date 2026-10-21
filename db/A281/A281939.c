/*
 * A281939  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x - y and
 *          3*z + w both squares, where x,y,z are nonnegative integers and w
 *          is an integer.
 * https://oeis.org/A281939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by brute‑force enumeration of quadruples (x,y,z,w)
   satisfying: x^2 + y^2 + z^2 + w^2 = n,
   x,y,z >= 0, w any integer,
   x >= y and x−y is a perfect square,
   3·z + w is a perfect square. */
#include <stdbool.h>
#include <stdint.h>

#define MAX_VAL 10000                 /* enough for n <= 10000 */

static bool init_sq = false;
static bool      is_sq[MAX_VAL + 1];
static unsigned short sq_root[MAX_VAL + 1];

static void init_tables(void)
{
    /* clear and fill tables */
    for (int i = 0; i <= MAX_VAL; ++i) {
        is_sq[i]   = false;
        sq_root[i] = 0;
    }
    for (int i = 0; i * i <= MAX_VAL; ++i) {
        int sq = i * i;
        is_sq[sq]   = true;
        sq_root[sq] = (unsigned short)i;
    }
}

seq_t a(idx_t n)
{
    if (!init_sq) {
        init_tables();
        init_sq = true;
    }

    seq_t count = (seq_t)0;

    for (idx_t x = 0; x * x <= n; ++x) {
        idx_t x2 = x * x;
        for (idx_t y = 0; y <= x; ++y) {
            idx_t y2 = y * y;
            if (x2 + y2 > n) break;

            idx_t diff = x - y;
            if (diff > MAX_VAL || !is_sq[diff]) continue;   /* diff must be a square */

            idx_t rem_xy = n - x2 - y2;
            for (idx_t z = 0; z * z <= rem_xy; ++z) {
                idx_t z2 = z * z;
                idx_t rem = rem_xy - z2;
                if (rem > MAX_VAL || !is_sq[rem]) continue;  /* rem must be a square */

                unsigned short s = sq_root[rem];        /* sqrt(rem), 0 if rem==0 */

                if (s == 0) {                            /* w must be 0 */
                    idx_t t = 3 * z;
                    if (t <= MAX_VAL && is_sq[t]) ++count;
                } else {
                    /* w = +s */
                    idx_t wpos = (idx_t)s;
                    idx_t t1 = 3 * z + wpos;
                    if (t1 <= MAX_VAL && is_sq[t1]) ++count;

                    /* w = -s */
                    int wneg = -(int)s;
                    int64_t t_signed = (int64_t)3 * z + wneg;
                    if (t_signed >= 0 && t_signed <= MAX_VAL &&
                        is_sq[(idx_t)t_signed]) ++count;
                }
            }
        }
    }

    return count;
}
