/*
 * A302159  Number of nX4 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 6*a(n-3) - 4*a(n-5) -7*a(n-6) - a(n-7)
 *            + 3*a(n-8) + 2*a(n-9) - a(n-10) -4*a(n-11) -3*a(n-12) + a(n-13)
 *            + 2*a(n-14) + 2*a(n-15) for n > 19 Initial values (1‑based):
 *            a(1) = 5, a(2) = 17, a(3) = 12, a(4) = 29, a(5) = 73, a(6) =
 *            156, a(7) = 361, a(8) = 865, a(9) = 1970, a(10)...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t known[20] = {
    (seq_t)0,          /* unused index 0 */
    (seq_t)5,          /* a(1) */
    (seq_t)17,         /* a(2) */
    (seq_t)12,         /* a(3) */
    (seq_t)29,         /* a(4) */
    (seq_t)73,         /* a(5) */
    (seq_t)156,        /* a(6) */
    (seq_t)361,        /* a(7) */
    (seq_t)865,        /* a(8) */
    (seq_t)1970,       /* a(9) */
    (seq_t)4530,       /* a(10) */
    (seq_t)10571,      /* a(11) */
    (seq_t)24394,      /* a(12) */
    (seq_t)56204,      /* a(13) */
    (seq_t)130230,     /* a(14) */
    (seq_t)301221,     /* a(15) */
    (seq_t)695541,     /* a(16) */
    (seq_t)1608549,    /* a(17) */
    (seq_t)3720177,    /* a(18) */
    (seq_t)8597848     /* a(19) */
};

seq_t a(idx_t n) {
    /* Static cache for previously computed values */
    static seq_t *buf = NULL;
    static size_t bufcap = 0;   /* allocated size (number of slots) */
    static idx_t max_comp = 0;  /* largest n computed so far */

    /* Ensure we have storage to hold up to n */
    if (n >= bufcap) {
        size_t newcap = n + 1;
        if (newcap < 20) newcap = 20;          /* at least capacity for initial terms */
        if (bufcap == 0) {
            buf = (seq_t *)malloc(sizeof(seq_t) * newcap);
        } else {
            buf = (seq_t *)realloc(buf, sizeof(seq_t) * newcap);
        }
        bufcap = newcap;
    }

    /* If already computed, just return the stored value */
    if (n <= max_comp) {
        return buf[n];
    }

    /* Compute from next missing index up to n */
    for (idx_t k = max_comp + 1; k <= n; ++k) {
        if (k <= 19) {
            /* Use the hard‑coded initial values */
            buf[k] = known[k];
        } else {
            /* Recurrence as per OEIS formula */
            seq_t val = buf[k-1] + buf[k-2] + 6*buf[k-3];
            val -= 4*buf[k-5];
            val -= 7*buf[k-6];
            val -= buf[k-7];
            val += 3*buf[k-8];
            val += 2*buf[k-9];
            val -= buf[k-10];
            val -= 4*buf[k-11];
            val -= 3*buf[k-12];
            val += buf[k-13];
            val += 2*buf[k-14];
            val += 2*buf[k-15];
            buf[k] = val;
        }
    }

    max_comp = n;
    return buf[n];
}
