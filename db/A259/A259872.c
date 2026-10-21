/*
 * A259872  a(0)=-1, a(1)=1; a(n) = n*a(n-1) + (n-2)*a(n-2) + Sum_{j=1..n-1}
 *          a(j)*a(n-j) + 2*Sum_{j=0..n-1} a(j)*a(n-1-j).
 * https://oeis.org/A259872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1, a(1) = 1; For n ≥ 2: a(n) = n*a(n-1) + (n-2)*a(n-2) +
 *            Σ_{j=1}^{n-1} a(j)*a(n-j) + 2*Σ_{j=0}^{n-1} a(j)*a(n-1-j)
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t upto = 0;                 /* highest index already computed */

    /* Ensure array large enough.  Use n+1 entries for index n. */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Handle the trivial cases explicitly. */
    if (n <= 1) {
        if (n == 0)
            cache[0] = (seq_t)-1;
        else
            cache[1] = (seq_t)1;
        upto = n;
        return cache[n];
    }

    /* Ensure that a(0) and a(1) are set when beginning the recurrence. */
    if (upto < 1) {
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)1;
        upto = 1;
    }

    /* Compute values up to n using the recurrence. */
    for (idx_t i = upto + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)i * cache[i - 1];
        seq_t term2 = (seq_t)(i - 2) * cache[i - 2];
        seq_t sum3 = 0;
        seq_t sum4 = 0;
        for (idx_t j = 1; j <= i - 1; ++j) {
            sum3 += cache[j] * cache[i - j];
        }
        for (idx_t j = 0; j <= i - 1; ++j) {
            sum4 += cache[j] * cache[i - 1 - j];
        }
        cache[i] = term1 + term2 + sum3 + sum4 * 2;
    }
    upto = n;
    return cache[n];
}
