/*
 * A338664  a(n) is the number of ways that n square tiles can be formed into
 *          two rectangles, such that those rectangles fit orthogonally into
 *          the minimal bounding square that can contain n such tiles, without
 *          overlap.
 * https://oeis.org/A338664
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

/* A338664: Count the number of unordered pairs of
   non‑trivial integer rectangles of sides (x≤y) and (z≤w) such
   that
          x*y + z*w = n,
          x , y , z , w ≤ ceil(sqrt(n)),
          x + z ≤ ceil(sqrt(n)).
   The function enumerates all factorizations of each area A
   and its complement B = n – A, builds the lists of admissible
   rectangle dimensions, and counts pairs satisfying the
   orthogonal‑fit condition. */

#include <stdint.h>

static idx_t ceil_sqrt(idx_t n)
{
    idx_t s = 0;
    while (s + 1 <= n / (s + 1))
        ++s;
    if (s * s < n)
        ++s;
    return s;
}

typedef struct { idx_t x; idx_t y; } rect_t;

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* n = 1 has no non‑trivial pair */

    const idx_t L = ceil_sqrt(n);

    /* Upper bound on the number of divisor pairs for n ≤ 10000
       (< 200), 256 is safely large. */
    rect_t rectA[256], rectB[256];
    int lenA, lenB;
    seq_t count = 0;

    for (idx_t areaA = 1; areaA <= n / 2; ++areaA) {
        idx_t areaB = n - areaA;

        /* Build rectangle list for areaA */
        lenA = 0;
        for (idx_t i = 1; i <= L && i <= areaA / i; ++i) {
            if (areaA % i == 0) {
                idx_t j = areaA / i;
                if (j <= L) {
                    rectA[lenA].x = i;
                    rectA[lenA].y = j;
                    ++lenA;
                }
            }
        }

        /* Build rectangle list for areaB */
        lenB = 0;
        for (idx_t i = 1; i <= L && i <= areaB / i; ++i) {
            if (areaB % i == 0) {
                idx_t j = areaB / i;
                if (j <= L) {
                    rectB[lenB].x = i;
                    rectB[lenB].y = j;
                    ++lenB;
                }
            }
        }

        if (areaA == areaB) {   /* unordered pairs from one list */
            for (int i = 0; i < lenA; ++i) {
                for (int j = i; j < lenA; ++j) {
                    if (rectA[i].x + rectA[j].x <= L)
                        ++count;
                }
            }
        } else {                /* pairs from distinct lists */
            for (int i = 0; i < lenA; ++i) {
                for (int j = 0; j < lenB; ++j) {
                    if (rectA[i].x + rectB[j].x <= L)
                        ++count;
                }
            }
        }
    }

    return count;
}
