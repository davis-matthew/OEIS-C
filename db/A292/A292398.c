/*
 * A292398  p-INVERT of A010892, where p(S) = 1 - S - S^2 - S^3.
 * https://oeis.org/A292398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 6*a(n-3) + a(n-4) + 4*a(n-5) + a(n-6)
 *            a(0) = 1, a(1) = 3, a(2) = 10, a(3) = 32, a(4) = 102, a(5) =
 *            323, a(6) = 1021
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array holding computed terms          */
static size_t cap   = 0;          /* allocated capacity (number of slots)  */
static size_t len   = 0;          /* number of terms currently computed   */

static void ensure_capacity(size_t need)
{
    if (cap <= need) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        /* zero new slots to avoid garbage */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }
}

/* p-INVERT of sequence A010892 (p(S)=1−S−S²−S³) */
seq_t a(idx_t n)
{
    /* Initial values for indices 0..6 */
    static const seq_t init[7] = { (seq_t)1, (seq_t)3, (seq_t)10,
                                   (seq_t)32, (seq_t)102, (seq_t)323,
                                   (seq_t)1021 };
    if (n < 7) return init[n];

    size_t idx = (size_t)n;
    ensure_capacity(idx);

    if (len < 7) {
        /* fill first 7 terms into cache */
        for (size_t i = len; i < 7; ++i) cache[i] = init[i];
        len = 7;
    }

    for (size_t i = len; i <= idx; ++i) {
        seq_t t = 4 * cache[i - 1];
        t -= cache[i - 2];
        t -= 6 * cache[i - 3];
        t += cache[i - 4];
        t += 4 * cache[i - 5];
        t += cache[i - 6];
        cache[i] = t;
    }
    len = idx + 1;            /* now all terms up to n are computed */
    return cache[idx];
}
