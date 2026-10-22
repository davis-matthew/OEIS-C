/*
 * A338336  Number of integer-sided disphenoids (isosceles tetrahedra) with
 *          triangle face perimeter n which can be used to build a
 *          kaleidocycle by connecting 6 congruent pieces into a cycle.
 * https://oeis.org/A338336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A338336
 *
 * a(n) = # of unordered integer triples (a,b,c) with
 *        c ≤ a ≤ b, a+b+c = n, satisfying
 *
 * -8*(a²-b²)²*(a²+b²) - 5*c⁶ + 11*(a²-b²)²*c² + 2*(a²+b²)*c⁴ ≥ 0.
 *
 * The function caches the values so that repeated calls are fast.
 */

#include <stdlib.h>   /* malloc, realloc, free, memset */
#include <string.h>   /* memset */

static seq_t *cnt = NULL;       /* count for each perimeter (index = n) */
static size_t cnt_len = 0;      /* allocated length of cnt */

/* Check the inequality for given sides using 128‑bit arithmetic.  */
static int valid_triple(idx_t a, idx_t b, idx_t c)
{
    unsigned __int128 a2 = (unsigned __int128)a * a;
    unsigned __int128 b2 = (unsigned __int128)b * b;
    unsigned __int128 diff = (a2 >= b2) ? (a2 - b2) : (b2 - a2);
    unsigned __int128 d2   = diff * diff;
    unsigned __int128 s    = a2 + b2;
    unsigned __int128 z    = (unsigned __int128)c * c;
    unsigned __int128 z2   = z * z;
    unsigned __int128 z3   = z2 * z;

    __int128 val = -(__int128)8 * ((__int128)d2 * (__int128)s)
                   - (__int128)5 * (__int128)z3
                   + (__int128)11 * (__int128)d2 * (__int128)z
                   + (__int128)2 * (__int128)s * (__int128)z2;

    return val >= 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n = 0, return 0 */

    /* Extend the cached array if needed */
    if (cnt_len <= n) {
        size_t old_len = cnt_len;
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cnt = (seq_t *)realloc(cnt, new_len * sizeof(seq_t));
        if (!new_cnt)                 /* allocation failure – return 0 */
            return 0;
        cnt = new_cnt;
        cnt_len = new_len;
        /* Zero the newly allocated part of the array */
        memset(cnt + old_len, 0, (new_len - old_len) * sizeof(seq_t));

        /* Old maximum perimeter already processed */
        idx_t old_max = old_len > 0 ? (old_len - 1) : 0;

        /* Enumerate all new triples whose perimeter exceeds old_max */
        if (n >= 3) {
            for (idx_t c = 1; c <= n - 2; ++c) {
                for (idx_t a = c; a <= (n - c) / 2; ++a) {
                    idx_t b_min = a;
                    idx_t b_max = n - c - a;          /* ensures a ≤ b ≤ b_max */
                    if (b_max < b_min)
                        continue;

                    idx_t start_b = b_min;
                    if (old_max >= 1) {
                        /* Minimum b that makes a+b+c > old_max */
                        idx_t need = (old_max + 1 > a + c) ? (old_max + 1 - (a + c)) : 0;
                        if (need > start_b)
                            start_b = need;
                    }

                    if (start_b > b_max)
                        continue;

                    for (idx_t b = start_b; b <= b_max; ++b) {
                        idx_t p = a + b + c;
                        if (valid_triple(a, b, c))
                            cnt[p] += (seq_t)1;
                    }
                }
            }
        }
    }

    return cnt[n];
}
