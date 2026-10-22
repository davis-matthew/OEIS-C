/*
 * A309045  Expansion of Product_{k>=0} (1 + x^(3^k) + x^(2*3^k) +
 *          x^(3^(k+1)))^(2^k).
 * https://oeis.org/A309045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from A(x) = (1 + x + x^2 + x^3) * A(x^3)^2:
   a(0) = 1;
   for n > 0,
     a(n) = Σ_{k=0..3, n >= k, (n-k)%3==0} Σ_{i=0..t} a(i) * a(t-i),
     where t = (n-k)/3. */
#include <stdlib.h>   /* malloc, realloc, NULL */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static idx_t last = 0;

    /* initialize on first call */
    if (seq == NULL) {
        cap = 1;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;   /* allocation failed */
        seq[0] = (seq_t)1;
        last = 0;
    }

    /* extend cache if necessary */
    if (n > last) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t new_cap = cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed */
            seq = tmp;
            cap = new_cap;
        }

        /* compute new terms */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k <= 3; ++k) {
                if (i >= (idx_t)k && ((i - (idx_t)k) % 3) == 0) {
                    idx_t t = (i - (idx_t)k) / 3;
                    for (idx_t j = 0; j <= t; ++j) {
                        sum += seq[j] * seq[t - j];
                    }
                }
            }
            seq[i] = sum;
        }
        last = n;
    }

    return seq[n];
}
