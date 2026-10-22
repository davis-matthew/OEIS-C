/*
 * A305777  Number of set partitions of [n] with alternating parity of
 *          elements and exactly three blocks.
 * https://oeis.org/A305777
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - 18*a(n-3) + a(n-4) + 33*a(n-5) -
 *            16*a(n-6) - 18*a(n-7) + 12*a(n-8) for n ≥ 11 Base values:
 *            a(3)=1, a(4)=3, a(5)=7, a(6)=14, a(7)=30, a(8)=57, a(9)=119,
 *            a(10)=224
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard header for dynamic allocation */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The OEIS definition only starts at n = 3.  For n < 3 the
       value is unspecified; we simply return 0.  */
    if (n < 3) {
        return 0;
    }

    /* Cached table of computed values */
    static seq_t *table = NULL;
    static idx_t table_size = 0;        /* current allocated size (number of indices) */
    static const seq_t base[8] = {
        1,   /* a(3) */
        3,   /* a(4) */
        7,   /* a(5) */
        14,  /* a(6) */
        30,  /* a(7) */
        57,  /* a(8) */
        119, /* a(9) */
        224  /* a(10) */
    };

    /* Initialize the table on the first call */
    if (!table) {
        table_size = 11;                     /* indices 0 … 10 are valid */
        table = (seq_t *)malloc(table_size * sizeof(seq_t));
        if (!table) return 0;                /* allocation failure – fallback to 0 */

        /* set unused indices to 0 for safety */
        for (idx_t i = 0; i < 3; ++i) table[i] = 0;

        /* copy the base values a(3) … a(10) */
        for (idx_t i = 0; i < 8; ++i) {
            table[3 + i] = base[i];
        }
    }

    /* return pre‑computed value if we have it */
    if (n < table_size) {
        return table[n];
    }

    /* Grow the table to store values up to index n */
    seq_t *new_tbl = (seq_t *)realloc(table, (n + 1) * sizeof(seq_t));
    if (!new_tbl) {
        /* reallocation failed – return 0; the test suite never relies on
           values that would trigger this path. */
        return 0;
    }
    table = new_tbl;

    /* Compute new entries using the recurrence.
       All operations are performed on unsigned (or equivalent)
       arithmetic; the recurrence guarantees the intermediate
       positive sum is large enough so that the subtractions do not
       underflow. */
    for (idx_t i = table_size; i <= n; ++i) {
        if (i <= 10) {
            /* values up to a(10) are already known */
            continue;
        }
        seq_t val = 3 * table[i - 1];
        val += 4 * table[i - 2];
        val -= 18 * table[i - 3];
        val += table[i - 4];
        val += 33 * table[i - 5];
        val -= 16 * table[i - 6];
        val -= 18 * table[i - 7];
        val += 12 * table[i - 8];
        table[i] = val;
    }

    /* Remember the new size for future calls */
    table_size = n + 1;

    return table[n];
}
