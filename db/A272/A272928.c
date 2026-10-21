/*
 * A272928  Partial sums of A147562.
 * https://oeis.org/A272928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0. Let f(n) = A147562(n). Then f(1) = 1; f(2k) = 4 * f(k)
 *            + 1; f(2k+1) = f(2k) + 4 * 3^{ popcount(2k+1) - 2 } for k≥1.
 *            Finally, a(n) = Σ_{i=1..n} f(i)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation. */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the population count of an unsigned 64‑bit integer. */
static unsigned popcnt_u64(idx_t x) {
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & 1ULL);
        x >>= 1ULL;
    }
    return c;
}

/* Compute a(n).  All values are stored in static caches so that
   calls need not be monotonic.  The cache grows as needed. */
seq_t a(idx_t n) {
    static seq_t *f_cache = NULL;   /* f(n) values, indexed by n */
    static seq_t *sum_cache = NULL; /* a(n) values, indexed by n */
    static size_t f_cap = 0, sum_cap = 0;
    static idx_t f_size = 0, sum_size = 0;

    /* Initialisation on first call. */
    if (f_cache == NULL) {
        f_cap = sum_cap = 16;          /* initial capacity */
        f_cache = (seq_t*) malloc((f_cap + 1) * sizeof(seq_t));
        sum_cache = (seq_t*) malloc((sum_cap + 1) * sizeof(seq_t));
        if (f_cache == NULL || sum_cache == NULL)
            return 0;                  /* allocation failure; undefined behaviour */
        f_cache[0] = 0;              /* not used but convenient */
        sum_cache[0] = 0;
        f_size = sum_size = 1;       /* we have values for index 0 only */
    }

    /* Grow the caches if required. */
    if (n >= f_size) {
        size_t need = n + 1;
        if (f_cap < need) {
            f_cap = f_cap ? f_cap * 2 : 16;
            while (f_cap < need) f_cap <<= 1;
            f_cache = (seq_t*)realloc(f_cache, (f_cap + 1) * sizeof(seq_t));
        }
        if (sum_cap < need) {
            sum_cap = sum_cap ? sum_cap * 2 : 16;
            while (sum_cap < need) sum_cap <<= 1;
            sum_cache = (seq_t*)realloc(sum_cache, (sum_cap + 1) * sizeof(seq_t));
        }

        /* Compute missing values. */
        for (idx_t i = f_size; i <= n; ++i) {
            seq_t fval;
            if (i == 1) {
                fval = 1;                 /* base case */
            } else if ((i & 1ULL) == 0ULL) {   /* even */
                fval = (seq_t)4 * f_cache[i / 2] + 1;
            } else {                     /* odd, i > 1 */
                unsigned pc = popcnt_u64(i);
                /* compute 4 * 3^{pc-2} safely in seq_t */
                seq_t inc = 4;
                if (pc > 2) {
                    seq_t pow3 = 1;
                    for (unsigned j = 0; j < pc - 2; ++j) pow3 *= 3;
                    inc *= pow3;
                }
                fval = f_cache[i - 1] + inc;
            }
            f_cache[i] = fval;
            sum_cache[i] = sum_cache[i - 1] + fval;
        }
        f_size = sum_size = n + 1;
    }

    return sum_cache[n];
}
