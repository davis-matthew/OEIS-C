/*
 * A250686  Number of (n+1) X (3+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) -331*a(n-2) +2439*a(n-3) -12049*a(n-4) *
 *            +42129*a(n-5) -107225*a(n-6) +201069*a(n-7) -277706*a(n-8) *
 *            +278928*a(n-9) -197984*a(n-10) +94032*a(n-11) -26784*a(n-12) *
 *            +3456*a(n-13) , n > 17 * Initial values: * a(1)=2541,
 *            a(2)=13906, a(3)=74631, a(4)=383440, a(5)=1848953, * a(6)=847...
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Known terms a(1)…a(18) */
    static const seq_t init[19] = {
        (seq_t)0,
        (seq_t)2541,          /* a(1) */
        (seq_t)13906,         /* a(2) */
        (seq_t)74631,         /* a(3) */
        (seq_t)383440,        /* a(4) */
        (seq_t)1848953,       /* a(5) */
        (seq_t)8474002,       /* a(6) */
        (seq_t)37322413,      /* a(7) */
        (seq_t)159438246,     /* a(8) */
        (seq_t)665575403,     /* a(9) */
        (seq_t)2731006288,    /* a(10) */
        (seq_t)11064772389,   /* a(11) */
        (seq_t)44421077632,   /* a(12) */
        (seq_t)177194842907,  /* a(13) */
        (seq_t)703795289106,  /* a(14) */
        (seq_t)2787900969927, /* a(15) */
        (seq_t)11027407758154,/* a(16) */
        (seq_t)43593798115489,/* a(17) */
        (seq_t)172349301152440/* a(18) */
    };

    /* Recurrence coefficients */
    static const int coeffs[13] = {
        27,   -331,  2439,  -12049, 42129,  -107225,
        201069, -277706, 278928, -197984, 94032, -26784, 3456
    };

    /* Memoisation table (1‑based). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest computed n */

    if (cache == NULL) {
        cache_len = 18;
        cache = (seq_t *)calloc((size_t)(cache_len + 1), sizeof(seq_t));
        for (idx_t i = 1; i <= cache_len; ++i)
            cache[i] = init[i];
    }

    if (n <= cache_len)
        return cache[n];

    /* Extend cache up to n. */
    idx_t old_len = cache_len;
    cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    cache_len = n;

    /* Compute missing terms using the recurrence. */
    for (idx_t pos = old_len + 1; pos <= n; ++pos) {
        seq_t sum_pos = 0;   /* positive contributions */
        seq_t sum_neg = 0;   /* positive contributions of |negative| coeffs */

        for (int j = 0; j < 13; ++j) {
            idx_t idx = pos - (idx_t)(j + 1);
            int c = coeffs[j];
            seq_t val = cache[idx];
            if (c >= 0)
                sum_pos = sum_pos + (seq_t)c * val;
            else
                sum_neg = sum_neg + (seq_t)(-c) * val;
        }
        cache[pos] = sum_pos - sum_neg;   /* final positive result */
    }

    return cache[n];
}
