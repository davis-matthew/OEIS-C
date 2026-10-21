/*
 * A279678  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = 7/4.
 * https://oeis.org/A279678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies a(0)=1, a(1)=-3, a(2)=4, a(3)=-4, a(4)=5,
 *            a(5)=-9; for n≥6, a(n) = -2*(a(n-1)+a(n-2)+a(n-3)). This follows
 *            from the rational generating function (1‑x)(1‑x⁴)/(1+2x+2x²+2x³)
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 49, i64 n <= 100,
 *            i128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static buffer for memoisation */
    static seq_t *buf = NULL;
    static size_t buf_cap = 0;   /* capacity of buf (number of elements) */
    static size_t buf_len = 0;   /* number of elements computed so far */

    /* ensure capacity at least n+1 */
    if (buf_len <= n) {
        size_t need = n + 1;
        if (buf_cap < need) {
            size_t new_cap = buf_cap ? buf_cap << 1 : 16;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* allocation failure – undefined for judge */
            buf = tmp;
            buf_cap = new_cap;
        }
        /* compute values from buf_len up to n */
        static const seq_t rhs[6] = {1, -1, 0, 0, -1, 1};
        for (size_t i = buf_len; i <= (size_t)n; ++i) {
            seq_t v = 0;
            /* contributions from previous terms */
            if (i >= 1) v -= 2 * buf[i - 1];
            if (i >= 2) v -= 2 * buf[i - 2];
            if (i >= 3) v -= 2 * buf[i - 3];
            /* add inhomogeneous part for first 6 indices */
            if (i < 6) v += rhs[(int)i];
            buf[i] = v;
        }
        buf_len = n + 1;
    }
    return buf[(size_t)n];
}
