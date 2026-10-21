/*
 * A243041  Number of length n+3 0..5 arrays with no four elements in a row
 *          with pattern abab (with a!=b) and new values 0..5 introduced in
 *          0..5 order.
 * https://oeis.org/A243041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A243041 – number of length n+3 0..5 arrays with no abab pattern.
   Recurrence (for n≥14):
   a(n) = 12*a(n-1) - 41*a(n-2) + 13*a(n-3) + 96*a(n-4) + 33*a(n-5)
          - 134*a(n-6) - 166*a(n-7) - 33*a(n-8) + 82*a(n-9) + 87*a(n-10)
          + 41*a(n-11) + 10*a(n-12) + a(n-13)
   Initial terms: a(1)=14, a(2)=47, a(3)=179, a(4)=759, a(5)=3516,
   a(6)=17463, a(7)=91409, a(8)=496910, a(9)=2773138, a(10)=15753844,
   a(11)=90561096, a(12)=524666847, a(13)=3055232669 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Coefficients for a(n)=Σ coeff[i]*a(n-i-1), i=0..12 */
    static const int coeff[13] = {
        12, -41, 13, 96, 33, -134, -166, -33, 82, 87, 41, 10, 1
    };

    /* Initial 13 terms */
    static const seq_t init[13] = {
        (seq_t)14,      (seq_t)47,
        (seq_t)179,     (seq_t)759,
        (seq_t)3516,    (seq_t)17463,
        (seq_t)91409,   (seq_t)496910,
        (seq_t)2773138, (seq_t)15753844,
        (seq_t)90561096,(seq_t)524666847,
        (seq_t)3055232669U
    };

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* number of terms stored (>=1) */
    static size_t cache_cap = 0;  /* allocated capacity (in terms) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 13;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_max = 13;
    }

    /* Return cached value if available */
    if (n <= cache_max)
        return cache[(size_t)(n - 1)];

    /* Compute successive terms until we reach the requested index */
    for (idx_t next = cache_max + 1; next <= n; ++next) {
        /* Ensure enough capacity */
        if (cache_max >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Separate positive and negative contributions to avoid underflow */
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        for (int i = 0; i < 13; ++i) {
            /* Index of a(next-i-1) in zero‑based cache array */
            idx_t k = next - i - 2;
            seq_t term = cache[(size_t)k];
            int c = coeff[i];
            if (c > 0)
                pos_sum += term * (seq_t)c;
            else
                neg_sum += term * (seq_t)(-c);
        }

        seq_t val_next = pos_sum - neg_sum;   /* pos_sum >= neg_sum by construction */
        cache[cache_max] = val_next;
        ++cache_max;
    }

    return cache[(size_t)(n - 1)];
}
