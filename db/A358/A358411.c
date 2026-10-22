/*
 * A358411  a(n) = Sum_{d|n} (d + n/d - 1)!/(d - 1)!.
 * https://oeis.org/A358411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d + n/d - 1)! / (d - 1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/* Utility: floor integer square root (uint64_t)                 */
/* ------------------------------------------------------------ */
static idx_t int_isqrt_u64(idx_t n)
{
    /* Find greatest x such that x*x <= n */
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {          /* avoid division by zero */
            lo = mid + 1;
            continue;
        }
        if (mid > n / mid)       /* mid*mid > n without overflow */
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return hi;
}

/* ------------------------------------------------------------ */
/* Factorial cache                                         */
/* ------------------------------------------------------------ */
static seq_t *fact_table = NULL;  /* fact_table[i] == i!   */
static size_t  fact_size  = 0;    /* size of the allocated table */

/* Ensure fact_table is populated up to n (inclusive). */
static void ensure_factorial_upto(idx_t n)
{
    if ((size_t)n < fact_size) return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_tbl = (seq_t *)realloc(fact_table, new_size * sizeof(seq_t));
    if (!new_tbl) exit(1);  /* fatal: out of memory */

    /* Initialize from the old size (or from scratch). */
    if (fact_size == 0) {
        new_tbl[0] = (seq_t)1;
        fact_size = 1;
    }
    for (size_t i = fact_size; i < new_size; ++i) {
        new_tbl[i] = new_tbl[i - 1] * (seq_t)i;
    }
    fact_table = new_tbl;
    fact_size  = new_size;
}

/* ------------------------------------------------------------ */
/* Sequence function                                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* below offset – defined as 0 */

    ensure_factorial_upto(n);  /* factorials up to n are ready */

    seq_t sum = (seq_t)0;
    idx_t sqrt_n = int_isqrt_u64(n);

    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;                     /* complementary divisor */
            idx_t up = d + e - 1;                /* (d + e - 1) */
            seq_t term = fact_table[up] / fact_table[d - 1];
            sum += term;

            if (e != d) {   /* add the term for divisor e */
                /* For e divisor, the upper index is the same: up */
                seq_t term2 = fact_table[up] / fact_table[e - 1];
                sum += term2;
            }
        }
    }
    return sum;
}
