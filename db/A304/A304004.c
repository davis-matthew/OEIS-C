/*
 * A304004  Number of n X 2 0..1 arrays with every element unequal to 0, 2, 3
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=4, a(3)=7, a(4)=13, a(5)=26,
 *            a(n)=a(n-1)+3*a(n-2)-4*a(n-4) for n>5
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Sequence entry function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset is 1, value for n==0 not used */
    static seq_t *cache = NULL;             /* 1‑based index; cache[0] unused */
    static size_t cap = 0;                  /* allocated size (including index 0) */
    static size_t computed = 0;             /* highest index currently computed */

    size_t need = (size_t)n;                /* n fits in size_t for n<=10000 */

    /* Resize cache if necessary */
    if (need + 1 > cap) {
        size_t newcap = cap == 0 ? 16 : cap * 2;
        while (newcap <= need) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure – return zero */
        cache = tmp;
        cap = newcap;
    }

    /* Compute terms up to 'need' if not already done */
    for (size_t i = computed + 1; i <= need; ++i) {
        if (i == 1)           cache[i] = (seq_t)1;
        else if (i == 2)      cache[i] = (seq_t)4;
        else if (i == 3)      cache[i] = (seq_t)7;
        else if (i == 4)      cache[i] = (seq_t)13;
        else if (i == 5)      cache[i] = (seq_t)26;
        else {
            seq_t term1 = cache[i - 1];
            seq_t term2 = ((seq_t)3) * cache[i - 2];
            seq_t term4 = ((seq_t)4) * cache[i - 4];
            cache[i] = term1 + term2 - term4;
        }
    }
    computed = need;
    return cache[need];
}
