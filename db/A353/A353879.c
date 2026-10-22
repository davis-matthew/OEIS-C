/*
 * A353879  Number of tilings of a 4 X n rectangle using right trominoes,
 *          dominoes and 1 X 1 tiles.
 * https://oeis.org/A353879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 14*a(n-1) + 183*a(n-2) - 37*a(n-3) - 1929*a(n-4) + 2419*a(n-5)
          - 212*a(n-6) - 333*a(n-7) + 25*a(n-8) + 15*a(n-9)
   with a(0)=1, a(1)=5, a(2)=189, a(3)=3633, a(4)=83374,
   a(5)=1817897, a(6)=40220893, a(7)=886130549, a(8)=19546906987. */
#include <stddef.h>
#include <stdint.h>

/* Accessible allocation helpers for arbitrary‑precision seq_t. */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 8 */
    static const seq_t base[9] = {
        (seq_t)1ULL,           /* a(0) */
        (seq_t)5ULL,           /* a(1) */
        (seq_t)189ULL,         /* a(2) */
        (seq_t)3633ULL,        /* a(3) */
        (seq_t)83374ULL,       /* a(4) */
        (seq_t)1817897ULL,     /* a(5) */
        (seq_t)40220893ULL,    /* a(6) */
        (seq_t)886130549ULL,   /* a(7) */
        (seq_t)19546906987ULL  /* a(8) */
    };

    /* n < 9 : return the pre‑computed value */
    if (n < 9) {
        return base[n];
    }

    /* Memoised table of computed values.
       The table is enlarged to the next power of two when needed. */
    static seq_t *table = NULL;        /* allocated array of seq_t */
    static size_t capacity = 0;        /* allocated size of table     */
    static idx_t max_cached = 0;       /* largest n for which a(n) is stored */

    /* Initial allocation */
    if (table == NULL) {
        capacity = 16;                 /* start with 16 slots */
        table = seq_alloc(capacity);
        if (table == NULL) return 0;   /* allocation failed – unlikely */

        /* Fill base values */
        for (size_t i = 0; i < 9; ++i) {
            table[i] = base[i];
        }
        max_cached = 8;
    }

    /* Grow the table if the requested index is beyond the current cache */
    if (n > max_cached) {
        size_t new_cap = capacity;
        while (new_cap <= n) {
            new_cap <<= 1;            /* double until it fits n */
        }
        seq_t *tmp = seq_realloc(table, capacity, new_cap);
        if (tmp == NULL) return 0;     /* allocation failed – unlikely */
        table = tmp;
        capacity = new_cap;

        /* Compute new terms sequentially */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            table[i] =
                (seq_t)14  * table[i-1] +
                (seq_t)183 * table[i-2] -
                (seq_t)37  * table[i-3] -
                (seq_t)1929* table[i-4] +
                (seq_t)2419* table[i-5] -
                (seq_t)212 * table[i-6] -
                (seq_t)333 * table[i-7] +
                (seq_t)25  * table[i-8] +
                (seq_t)15  * table[i-9];
        }
        max_cached = n;
    }

    return table[n];
}
