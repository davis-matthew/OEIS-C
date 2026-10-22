/*
 * A375388  A family of squares S(m), m > 0, read by squares and then by rows;
 *          square S(1) is [1, 1; 1, 1]; for m > 0, square S(m+1) is obtained
 *          by replacing each subsquare [t, u; v, w] in S(m) by [t, t+u, u;
 *          t+v, t+u+v+w, u+w; v, v+w, w].
 * https://oeis.org/A375388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the value read by flattening S(1), S(2), … row‑wise.
 *            S(m) is a square of side length 2^{m-1}+1. Its entry (i,j)
 *            (0‑based) equals v_m[i] * v_m[j] where v_m is a vector of length
 *            2^{m-1}+1 defined recursively by v_1 = [1,1] v_{m+1}[0] = v_m[0]
 *            v_{m+1}[2k] = v_m[k] v_{m+1}[2k+1] = v_m[k] + v...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t get_val(idx_t m, idx_t idx)
{
    /* Recursive evaluation of the vector value v_m[idx] */
    if (m == 1)                     /* base: v_1 = [1,1] */
        return (seq_t)1;

    if ((idx & 1) == 0)             /* even position: copy from previous */
        return get_val(m - 1, idx >> 1);

    /* odd position: sum of two neighbours from previous */
    seq_t a = get_val(m - 1, (idx - 1) >> 1);
    seq_t b = get_val(m - 1, (idx + 1) >> 1);
    return a + b;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined; return 0 for safety */
        return (seq_t)0;

    idx_t m = 1;          /* current square index */
    idx_t len = 2;        /* side length of current square (S(m)) */
    idx_t total = 0;      /* number of terms before the current square */

    /* Find the square that contains term n */
    while (1) {
        idx_t terms = len * len;           /* number of terms in this square */
        if (total + terms >= n)            /* n lies in this square */
            break;
        total += terms;                    /* skip this square */
        len = 2 * len - 1;                 /* side length for next square */
        m++;
    }

    idx_t offset = n - 1 - total;          /* 0‑based offset inside S(m) */
    idx_t r = offset / len;               /* row index (0‑based)   */
    idx_t c = offset % len;               /* column index (0‑based) */

    seq_t vr = get_val(m, r);
    seq_t vc = get_val(m, c);

    return vr * vc;                        /* outer product element */
}
