/*
 * A356999  a(n) = 2*A356988(n) - n.
 * https://oeis.org/A356999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A356988(n) - n Sequence defined by the piecewise
 *            formulas from OEIS: a(F(k) + j) = F(k-3) + j, 0 ≤ j ≤ F(k-2), k
 *            ≥ 2 a(L(k) + j) = F(k) - j, 0 ≤ j ≤ F(k-2), k ≥ 1 with F(-1)=1,
 *            F(0)=0, F(1)=1, … and L(0)=2, L(1)=1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   grow_array_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/grow_array_to.h"

/*--- dynamic Fibonacci table --------------------------------------------*/
static seq_t *fib_tab = NULL;   /* Fibonacci numbers F(0),F(1),…      */
static size_t fib_len = 0;      /* number of filled entries            */
static size_t fib_cap = 0;      /* allocated capacity                  */

/* Ensure Fibonacci table contains values up to at least the first entry > n. */
static void ensure_fib(idx_t n)
{
    if (fib_tab == NULL) {
        grow_array_to(&fib_tab, &fib_cap, 3);
        fib_tab[0] = (seq_t)0;  /* F(0) */
        fib_tab[1] = (seq_t)1;  /* F(1) */
        fib_tab[2] = (seq_t)1;  /* F(2) */
        fib_len = 3;
    }
    /* Extend until F(k) > n, guard against overflow. */
    while (fib_tab[fib_len-1] <= (seq_t)n) {
        seq_t next = fib_tab[fib_len-1] + fib_tab[fib_len-2];
        if (next <= fib_tab[fib_len-1])   /* overflow detected */
            break;
        grow_array_to(&fib_tab, &fib_cap, fib_len + 1);
        fib_tab[fib_len++] = next;
    }
}

/*--- dynamic Lucas table -----------------------------------------------*/
static seq_t *luc_tab = NULL;   /* Lucas numbers L(0),L(1),…          */
static size_t luc_len = 0;      /* number of filled entries            */
static size_t luc_cap = 0;      /* allocated capacity                  */

static void ensure_luc(idx_t n)
{
    if (luc_tab == NULL) {
        grow_array_to(&luc_tab, &luc_cap, 3);
        luc_tab[0] = (seq_t)2;  /* L(0) */
        luc_tab[1] = (seq_t)1;  /* L(1) */
        luc_tab[2] = (seq_t)3;  /* L(2) */
        luc_len = 3;
    }
    while (luc_tab[luc_len-1] <= (seq_t)n) {
        seq_t next = luc_tab[luc_len-1] + luc_tab[luc_len-2];
        if (next <= luc_tab[luc_len-1])   /* overflow detected */
            break;
        grow_array_to(&luc_tab, &luc_cap, luc_len + 1);
        luc_tab[luc_len++] = next;
    }
}

/* Ensure fib_tab has at least `need` elements. */
static void extend_fib_to_size(size_t need)
{
    while (fib_len < need) {
        seq_t next = fib_tab[fib_len-1] + fib_tab[fib_len-2];
        if (next <= fib_tab[fib_len-1])   /* overflow detected */
            break;
        grow_array_to(&fib_tab, &fib_cap, fib_len + 1);
        fib_tab[fib_len++] = next;
    }
}

/*--- sequence function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; safety check */

    ensure_fib(n);
    ensure_luc(n);

    /* We may need Fibonacci numbers up to the largest used Lucas index. */
    if (luc_len > fib_len)
        extend_fib_to_size(luc_len);

    seq_t sn = (seq_t)n;

    /* Ascending segments (k ≥ 2) */
    for (idx_t k = 2; k < fib_len; ++k) {
        seq_t fk = fib_tab[k];
        seq_t block_len = fib_tab[k-2];           /* F(k-2) */
        if (sn >= fk && sn <= fk + block_len) {
            if (k == 2)                      /* F(k-3)=F(-1)=1 */
                return (seq_t)1;
            seq_t base = fib_tab[k-3];          /* F(k-3) */
            seq_t j = sn - fk;
            return base + j;
        }
    }

    /* Descending segments (k ≥ 1) */
    for (idx_t k = 1; k < luc_len; ++k) {
        seq_t lk = luc_tab[k];
        seq_t block_len;
        if (k >= 2)
            block_len = fib_tab[k-2];      /* F(k-2) */
        else
            block_len = (seq_t)1;          /* F(-1)=1 for k=1 */

        if (sn >= lk && sn <= lk + block_len) {
            seq_t j = sn - lk;
            seq_t fk = fib_tab[k];           /* F(k) */
            return fk - j;
        }
    }

    /* Should never happen for n ≥ 1 */
    return 0;
}
