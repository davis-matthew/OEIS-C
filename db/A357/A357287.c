/*
 * A357287  a(n) = number of subsets S of {1,2,...,n} having more than 2
 *          elements such that (sum of least three elements of S) = max(S).
 * https://oeis.org/A357287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A357287
    Recurrence: a(n) = 2*a(n-1) + a(n-2) - 6*a(n-4) - 4*a(n-5) + 16*a(n-6) - 8*a(n-7),
    with a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=0, a(6)=4.
    Computes the number of subsets S ⊆ {1,…,n} with |S|>2, sum of the three least
    elements of S equals max(S).  Uses memoised dynamic programming.  O(n) time. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 6 */
    const seq_t base[7] = {0, 0, 0, 0, 0, 0, 4};

    /* --- handle small n without allocation --- */
    if (n < 7)
        return base[n];

    /* --- memoisation table --- */
    static seq_t *tab = NULL;    /* array of computed a(k) for k = 0 … current_max */
    static idx_t  sz   = 0;      /* current allocated size (number of elements) */

    /* --- initialise or enlarge the table --- */
    if (tab == NULL) {
        /* first call: allocate space up to max(7, n+1) */
        idx_t alloc = n + 1;
        if (alloc < 7) alloc = 7;
        tab = (seq_t *)malloc(alloc * sizeof(seq_t));
        if (tab == NULL) exit(1);           /* allocation failure */
        for (idx_t i = 0; i < 7; ++i)      /* copy known base values   */
            tab[i] = base[i];
        sz = alloc;
        /* compute remaining values up to n */
        for (idx_t k = 7; k <= n; ++k) {
            seq_t val = 0;
            val += 2 * tab[k - 1];
            val += tab[k - 2];
            val -= 6 * tab[k - 4];
            val -= 4 * tab[k - 5];
            val += 16 * tab[k - 6];
            val -= 8 * tab[k - 7];
            tab[k] = val;
        }
        return tab[n];
    }

    /* If requested n is higher than current table limits, extend it */
    if (sz <= n) {
        idx_t old_sz = sz;
        sz = n + 1;
        seq_t *new_tab = (seq_t *)realloc(tab, sz * sizeof(seq_t));
        if (new_tab == NULL) exit(1);      /* allocation failure */
        tab = new_tab;
        /* Fill any missing base entries (only possible if old_sz < 7) */
        if (old_sz < 7) {
            for (idx_t i = old_sz; i < 7; ++i)
                tab[i] = base[i];
        }
        /* Compute new values from old_sz up to n */
        for (idx_t k = old_sz; k <= n; ++k) {
            if (k < 7) continue;          /* base values already set */
            seq_t val = 0;
            val += 2 * tab[k - 1];
            val += tab[k - 2];
            val -= 6 * tab[k - 4];
            val -= 4 * tab[k - 5];
            val += 16 * tab[k - 6];
            val -= 8 * tab[k - 7];
            tab[k] = val;
        }
    }

    return tab[n];
}
