/*
 * A348285  a(1) = 2; for n > 1, let a(k) be a divisor > 1 of n appearing in
 *          all previous terms where k is as large as possible. Then a(n) = n
 *          - k. If no such k exists then a(n) = n - m, where a(m) = 1 and m
 *          is as large as possible.
 * https://oeis.org/A348285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n>1, let k be the largest index < n such that
 *            a(k)>1 and a(k) divides n. Then a(n) = n - k. If no such k
 *            exists, let m be the largest index < n with a(m)=1, and a(n) = n
 *            - m
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* External linkage for the sequence function */
seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *seq = NULL;          /* array of already computed a(i), 1‑indexed */
    static size_t cap = 0;             /* current capacity of seq array */
    static idx_t len = 0;              /* how many values are stored (excluding index 0) */
    static idx_t last_one = 0;         /* largest index with a(i) == 1 */

    /* base case */
    if (n == 1) return (seq_t)2;

    /* ensure capacity for indices up to n (1‑indexed) */
    while (cap <= (size_t)n) {
        size_t new_cap = (cap == 0) ? 1 : cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();            /* memory allocation failure */
        seq = tmp;
        cap = new_cap;
    }

    /* initialise a(1) if not done yet */
    if (len < 1) {
        seq[1] = (seq_t)2;            /* a(1) = 2 */
        len = 1;
    }

    /* compute terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t ai = 0;
        bool found = false;
        for (idx_t k = i - 1; k > 0; --k) {
            seq_t val = seq[k];
            if (val > (seq_t)1 && ((seq_t)i % val == 0)) {
                ai = (seq_t)(i - k);
                found = true;
                break;
            }
        }
        if (!found) {
            ai = (seq_t)(i - last_one);
        }
        seq[i] = ai;
        if (ai == (seq_t)1) last_one = i;
    }

    len = n;
    return seq[n];
}
