/*
 * A351581  Four-column table read by rows giving quadruples of integers
 *          [w,x,y,z] with w > 0, x > 1, y > 1 and z > 0 such that y^2 - y -
 *          x*z = 0 and x^2 = w*y, sorted by y then by x.
 * https://oeis.org/A351581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 868, u32 n <= 2720, u64 n <= 2720,
 *            u128 n <= 2720, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all solutions of y^2 - y = x*z and x^2 = w*y;
   sorting by y then x, then concatenating each row w,x,y,z.
   This yields the OEIS sequence A351581 with offset 1. */
#include <stdlib.h>

static int cmp_idx_t(const void *a, const void *b) {
    idx_t va = *(const idx_t*)a;
    idx_t vb = *(const idx_t*)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n<1, but just in case */
    idx_t count = 0;               /* number of terms produced so far */

    for (idx_t y = 2; ; ++y) {     /* start from y=2 (since y>1) */
        idx_t N = y * (y - 1);     /* y*(y-1), safe in idx_t for small y */
        /* collect all divisors d of N with d>1 */
        size_t maxdiv = 2 * (size_t)((size_t)(y) + 1); /* rough upper bound */
        idx_t *divs = (idx_t*)malloc(maxdiv * sizeof(idx_t));
        size_t dcnt = 0;
        for (idx_t d = 1; d * d <= N; ++d) {
            if (N % d == 0) {
                idx_t d1 = d;
                idx_t d2 = N / d;
                if (d1 > 1 && dcnt < maxdiv) divs[dcnt++] = d1;
                if (d2 != d1 && d2 > 1 && dcnt < maxdiv) divs[dcnt++] = d2;
            }
        }
        if (dcnt == 0) { free(divs); continue; }

        /* sort divisors ascending */
        qsort(divs, dcnt, sizeof(idx_t), cmp_idx_t);

        /* iterate over eligible x values (sorted by x ascending) */
        for (size_t i = 0; i < dcnt; ++i) {
            idx_t x = divs[i];
            /* Check x^2 divisible by y */
            if ((((seq_t)x) * ((seq_t)x)) % ((seq_t)y) != 0) continue;

            seq_t w = ((seq_t)x) * ((seq_t)x) / ((seq_t)y);   /* x^2 / y */
            seq_t z = ((seq_t)N) / ((seq_t)x);                /* (y^2 - y) / x */

            /* output the row w,x,y,z sequentially */
            if (++count == n) { free(divs); return w; }
            if (++count == n) { free(divs); return (seq_t)x; }
            if (++count == n) { free(divs); return (seq_t)y; }
            if (++count == n) { free(divs); return z; }
        }
        free(divs);
    }
}
