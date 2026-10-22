/*
 * A324631  Number of permutations p of [n] such that four is the maximum of
 *          the number of elements in any integer interval
 *          [p(i)..i+n*[i<p(i)]].
 * https://oeis.org/A324631
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a_n = 2*a_{n-1} + a_{n-2} - a_{n-3} - 2*a_{n-4} - a_{n-5}             */
/* for n ≥ 9, with initial values a(4)=15, a(5)=31, a(6)=60,               */
/* a(7)=113, a(8)=215.  (For 4 ≤ n ≤ 8 the recurrence includes the RHS   */
/* coefficients 15, 1, –17, –23, –10 from the generating function.)       */

#include <stdlib.h>

static seq_t *table = NULL;     /* memoisation table */
static idx_t table_size = 0;    /* current allocated size (>=1) */

static void init_table(void)
{
    if (table == NULL) {
        table = (seq_t *)malloc(9 * sizeof(seq_t)); /* indices 0..8 */
        table_size = 9;
        for (idx_t i = 0; i < 4; ++i) table[i] = 0;
        table[4] = (seq_t)15;
        table[5] = (seq_t)31;
        table[6] = (seq_t)60;
        table[7] = (seq_t)113;
        table[8] = (seq_t)215;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;            /* values below offset are 0 */
    init_table();                   /* Ensure table is initialised */

    if (n >= table_size) {          /* Need to enlarge and compute more terms */
        idx_t old_size = table_size;
        idx_t new_size = n + 1;     /* allocate up to n */
        seq_t *tmp = (seq_t *)realloc(table, new_size * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure → 0 (unlikely in tests) */
        table = tmp;
        /* Compute missing terms from old_size to n */
        for (idx_t k = old_size; k <= n; ++k) {
            if (k <= 8) continue;  /* already set */
            seq_t sum = table[k-1];
            sum += table[k-1];                     /* 2 * a[k-1] */
            sum += table[k-2];                     /* + a[k-2] */
            sum -= table[k-3];                     /* - a[k-3] */
            seq_t t = table[k-4];
            t += t;                                 /* 2 * a[k-4] */
            sum -= t;                               /* - 2*a[k-4] */
            sum -= table[k-5];                       /* - a[k-5] */
            table[k] = sum;                          /* RHS is 0 for k≥9 */
        }
        table_size = new_size;
    }
    return table[n];
}
