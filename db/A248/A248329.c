/*
 * A248329  Square array read by antidiagonals downwards: super Patalan
 *          numbers of order 7.
 * https://oeis.org/A248329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(0,0)=1; T(n,k)=T(n-1,k)*(49*n-7)/(n+k) for n>0, k>=0
 *            T(n,k)=T(n,k-1)*(49*k-42)/(n+k) for k>0, n>=0 a(n) is the value
 *            read by antidiagonals downwards
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   prefix_before, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"
#include "../utils/prefix_before.h"

static idx_t currentS   = 0;          /* largest diagonal index computed */
static seq_t *table      = NULL;      /* flat array of size (currentS+1)^2 */
static size_t table_cap = 0;          /* number of seq_t elements allocated */

/* returns s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
static inline idx_t diag_of(idx_t n)
{
    double approx = sqrt((double)n * 8.0 + 1.0);
    idx_t s = (idx_t)((approx - 1.0) / 2.0);

    /* adjust for rounding errors */
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;
    return s;
}

/* compute or grow cache up to diagonal s */
static void ensure_cache(idx_t s)
{
    if (table == NULL) {                     /* initial allocation for n=0 */
        table = seq_realloc(NULL, 0, 1);
        if (table == NULL) return;
        table[0] = (seq_t)1;
        table_cap = 1;
        currentS = 0;
    }

    if (s <= currentS) return;              /* already covered */

    idx_t oldS   = currentS;
    idx_t newS   = s;
    size_t oldCap = (size_t)(oldS + 1) * (oldS + 1);
    size_t newCap = (size_t)(newS + 1) * (newS + 1);

    seq_t *tmp = seq_realloc(table, oldCap, newCap);
    if (tmp == NULL) return;                /* allocation failure */
    table = tmp;
    table_cap = newCap;

    /* fill new entries */
    for (idx_t i = 0; i <= newS; ++i) {
        for (idx_t j = 0; j <= newS; ++j) {
            idx_t pos = i * (newS + 1) + j;
            if (i == 0 && j == 0) {
                table[pos] = (seq_t)1;
            } else if (i == 0) {                /* first row */
                seq_t prev = table[0 * (newS + 1) + (j - 1)];
                seq_t num  = (seq_t)(49 * j - 42);
                seq_t den  = (seq_t)j;
                table[pos] = (prev * num) / den;
            } else if (j == 0) {               /* first column */
                seq_t prev = table[(i - 1) * (newS + 1) + 0];
                seq_t num  = (seq_t)(49 * i - 7);
                seq_t den  = (seq_t)i;
                table[pos] = (prev * num) / den;
            } else {                          /* interior */
                seq_t prev = table[(i - 1) * (newS + 1) + j];
                seq_t num  = (seq_t)(49 * i - 7);
                seq_t den  = (seq_t)(i + j);
                table[pos] = (prev * num) / den;
            }
        }
    }
    currentS = newS;
}

seq_t a(idx_t n)
{
    idx_t s = diag_of(n);                          /* diagonal index */
    ensure_cache(s);                               /* make sure data is ready */
    if (table == NULL) return (seq_t)0;            /* safety */

    idx_t k   = n - prefix_before(s);              /* offset inside the diagonal */
    idx_t i   = k;                                 /* row (n) */
    idx_t j   = s - k;                             /* column (k) */

    return table[i * (currentS + 1) + j];
}
