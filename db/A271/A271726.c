/*
 * A271726  Let f(x) = 1 -x^3+ Sum_{j>=2} (x^(2^j)-x^(1+2^j)). Then a(n) is n
 *          times the coefficient of x^n in the expansion of log(f(x)).
 * https://oeis.org/A271726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n * coefficient of x^n in log(1 - x^3 + Σ_{j≥2} (x^{2^j}
 *            - x^{1+2^j}))
 * Range:     i8 n <= 23, i16 n <= 51, i32 n <= 109, i64 n <= 226,
 *            i128 n <= 459, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from the logarithm of a power series):          */
/*   a(n) = n*d(n) - Σ_{k=1}^{n-1} a(k) * d(n‑k)                      */
/* where d(m) = -1 if m==3, +1 if m is a power of two ≥4,             */
/*              -1 if m-1 is a power of two ≥4, 0 otherwise.           */

#include <stdlib.h>
#include <stdint.h>

/* helper: test if an unsigned 64‑bit integer is a power of two */
static inline int is_power_of_two_idx(idx_t v) {
    return v && !(v & (v-1));
}

/* Memoised array of a(n) values (1‑based). 0‑th element always 0. */
static seq_t *a_cache = NULL;
static idx_t cache_size = 0;

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for 0‑based calls */

    /* Allocate cache on first use. */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!a_cache) return (seq_t)0;                /* OOM guard */
        a_cache[0] = 0;
        cache_size = 0;
    }

    /* Grow cache if necessary. */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* OOM guard */
        a_cache = tmp;

        /* Compute a(i) for i = cache_size+1 .. n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t sum = 0;

            /* Contribution from d(3) = -1 */
            if (i > 3) {
                sum += a_cache[i - 3] * (seq_t)-1;
            }

            /* Contributions from powers of two and 1+powers of two */
            idx_t pow = 4;
            while (pow <= i - 1) {
                seq_t term1 = a_cache[i - pow];
                sum += term1;                      /* d(pow) = +1 */

                if (pow + 1 <= i - 1) {
                    sum -= a_cache[i - (pow + 1)]; /* d(pow+1) = -1 */
                }

                /* Prevent overflow of pow << 1 */
                if (pow > (i - 1) / 2) break;
                pow <<= 1;
            }

            /* Compute d(i) */
            seq_t d;
            if (i == 3) {
                d = (seq_t)-1;
            } else if (i >= 4 && is_power_of_two_idx(i)) {
                d = (seq_t)1;
            } else if (i >= 5 && is_power_of_two_idx(i - 1)) {
                d = (seq_t)-1;
            } else {
                d = 0;
            }

            a_cache[i] = (seq_t)i * d - sum;
        }

        cache_size = n;
    }

    return a_cache[n];
}
