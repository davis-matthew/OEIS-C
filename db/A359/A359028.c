/*
 * A359028  Integers m such that A006218(m+1)/(m+1) > A006218(m)/m.
 * https://oeis.org/A359028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that * d(m+1) * m > Σ_{k=1..m}
 *            d(k) * where d(k) is the number of divisors of k
 * Range:     u8 n <= 37, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   divcnt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/divcnt.h"

/* --------- static storage -------------------------------------------- */
static seq_t *table   = NULL;   /* stored m values */
static size_t table_cap = 0;    /* array capacity */
static size_t table_len = 0;    /* number of stored terms */

static idx_t last_m   = 0;      /* last m that was processed   */
static seq_t sum_div  = 0;      /* Σ_{k=1..last_m} d(k) */

/* --------- sequence generator ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not defined for n = 0 */

    /* enlarge table if needed */
    if (table_len < (size_t)n)
    {
        if (n > table_cap)
        {
            size_t new_cap = table_cap ? table_cap * 2 : 16;
            if (new_cap < (size_t)n) new_cap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            table = tmp;
            table_cap = new_cap;
        }

        /* generate m values until we have n terms */
        while (table_len < (size_t)n)
        {
            /* next candidate m */
            ++last_m;

            /* add d(m) to the running sum */
            seq_t d_m = divcnt(last_m);
            sum_div += d_m;

            /* compute d(m+1) and test condition d(m+1)*m > sum_div */
            seq_t d_next = divcnt(last_m + 1);

            /* use division to avoid overflow of the product */
            if (d_next > sum_div / last_m)
            {
                if (table_len == table_cap)
                {
                    size_t new_cap = table_cap * 2;
                    seq_t *tmp2 = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
                    if (!tmp2) return 0;
                    table = tmp2;
                    table_cap = new_cap;
                }
                table[table_len++] = last_m;
            }
        }
    }
    return table[n - 1];
}
