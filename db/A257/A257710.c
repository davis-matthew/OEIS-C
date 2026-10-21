/*
 * A257710  Numbers n such that T(n) + T(n+1) + ... + T(n+36) is a square,
 *          where T = A000217 (triangular numbers).
 * https://oeis.org/A257710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A257710
 *
 *  a(n) = a(n−1) + 7398*a(n−8) − 7398*a(n−9) − a(n−16) + a(n−17)
 *
 *  Initial values:
 *    a(1) =  5
 *    a(2) = 32
 *    a(3) = 291
 *    a(4) = 661
 *    a(5) = 4102
 *    a(6) = 8515
 *    a(7) = 13685
 *    a(8) = 113558
 *    a(9) = 182368
 *    a(10)= 377701
 *    a(11)= 2290342
 *    a(12)= 5027232
 *    a(13)= 30483491
 *    a(14)= 63130838
 *    a(15)= 101378488
 *    a(16)= 840238915
 *    a(17)= 1349295285
 *
 *  The sequence is defined for n≥1.  a(0) is returned as 0 for
 *  convenience.
 */

#include <stdlib.h>
#include <stdint.h>

/*  seq_t, idx_t are defined by the build environment.
 *
 *  The function is memoised: fresh values are computed only when
 *  needed and stored in a static array.  The array is grown as
 *  necessary; it is never freed (intentionally, because the
 *  sequence is queried for a very small number of values per run).  */

static seq_t *seq = NULL;          /* 1‑based index, seq[0] unused */
static size_t seq_used = 0;        /* number of values stored (>= seq_len) */
static size_t seq_len  = 0;        /* highest index that has been computed */

/* initialise the table with the first 17 terms */
static void init_table(void)
{
    seq[1]  = (seq_t)5;
    seq[2]  = (seq_t)32;
    seq[3]  = (seq_t)291;
    seq[4]  = (seq_t)661;
    seq[5]  = (seq_t)4102;
    seq[6]  = (seq_t)8515;
    seq[7]  = (seq_t)13685;
    seq[8]  = (seq_t)113558;
    seq[9]  = (seq_t)182368;
    seq[10] = (seq_t)377701;
    seq[11] = (seq_t)2290342;
    seq[12] = (seq_t)5027232;
    seq[13] = (seq_t)30483491;
    seq[14] = (seq_t)63130838;
    seq[15] = (seq_t)101378488;
    seq[16] = (seq_t)840238915;
    seq[17] = (seq_t)1349295285;
    seq_len = 17;
    seq_used = 17;
}

/* expand the allocation to hold at least indices 0…need-1
 * (we always keep space for a[0] but never use it). */
static void ensure_capacity(idx_t need)
{
    if (seq != NULL)
        return;

    /* first allocation – give a little extra space than needed */
    size_t init_cap = (size_t)need + 1;
    if (init_cap < 64)
        init_cap = 64;      /* small buffer for speed */

    seq = (seq_t*)malloc(init_cap * sizeof(seq_t));
    if (seq == NULL) {
        /* in an extreme unlikely case malloc fails – the build will fail anyway */
        abort();
    }
    seq_used = init_cap - 1;
    init_table();
}

/* grow the array to at least 'need' (1‑based index) */
static void grow_if_needed(idx_t need)
{
    if ((size_t)need <= seq_used)
        return;

    size_t new_cap = seq_used + 1;
    if (new_cap < need + 1)
        new_cap = (size_t)need + 1;
    /* double the capacity to keep amortised O(1) per extension */
    if (new_cap < (seq_used + 1) * 2)
        new_cap = (seq_used + 1) * 2;

    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        abort();
    }
    seq = tmp;
    seq_used = new_cap - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1 but harmless */

    /* make sure we have storage up to 'n' */
    ensure_capacity(n);
    if ((size_t)n > seq_used) {
        grow_if_needed(n);
    }
    if ((size_t)n <= seq_len)   /* already computed */
        return seq[n];

    /* compute missing values up to n */
    for (idx_t k = seq_len + 1; k <= n; ++k)
    {
        /* recurrence: a(k) = a(k−1) + 7398*a(k−8) − 7398*a(k−9)
         *              − a(k−16) + a(k−17)   */
        seq_t term = seq[k-1];
        term += (seq_t)7398 * seq[k-8];
        term -= (seq_t)7398 * seq[k-9];
        term -= seq[k-16];
        term += seq[k-17];
        seq[k] = term;
    }
    seq_len = n;
    return seq[n];
}
