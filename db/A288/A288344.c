/*
 * A288344  Expansion of 1 / ((1-x)^2*(1-x^2)*(1-x^3)*...*(1-x^9)).
 * https://oeis.org/A288344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts ≤ 9, where part 1 *
 *            appears in two distinct kinds. The generating function is * 1 /
 *            ((1-x)^2 * Π_{i=2}^9 (1-x^i)). Equivalently, it is the
 *            coefficient * of x^n in that product. This implementation uses
 *            standard * coin‑change dynamic programming with two...
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 179, u64 n <= 2354,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *table = NULL;      /* Cached values a(0)…a(current_max)   */
static idx_t  table_size = 0;    /* Number of valid entries in table    */

/* Coin values: two 1's followed by 2…9 */
static const idx_t coin_vals[10] = {1, 1, 2, 3, 4, 5, 6, 7, 8, 9};

seq_t a(idx_t n)
{
    /* Grow table if needed */
    if (n >= table_size) {
        idx_t new_size = n + 1;
        seq_t *new_tbl = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_tbl) return 0;  /* memory failure – unlikely in tests */

        /* Initialise all entries to zero (necessary for arbitrary types) */
        for (idx_t i = 0; i < new_size; ++i)
            new_tbl[i] = 0;
        new_tbl[0] = 1;            /* empty partition */

        /* Compute values by the coin‑change DP */
        for (idx_t ci = 0; ci < 10; ++ci) {
            idx_t val = coin_vals[ci];
            for (idx_t i = val; i <= n; ++i) {
                new_tbl[i] += new_tbl[i - val];
            }
        }

        /* Replace old table and free memory */
        free(table);
        table      = new_tbl;
        table_size = new_size;
    }

    return table[n];
}
