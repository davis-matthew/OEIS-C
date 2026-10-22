/*
 * A345866  Nonnegative integers ordered by lowest number value per letter (in
 *          American English) and then, in case of ties, numerically.
 * https://oeis.org/A345866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A345866: a(n) = nth smallest nonnegative integer ordered by n/letter_count(n),
   ties broken numerically.  The routine builds a cache of the ordering up
   to a prescribed limit (200 000) and returns the requested element. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Number of letters for units (0‑9), teens (10‑19), tens (20‑90), and
   scale suffixes (thousand, million, ...). */
static const unsigned char unit_len[10]   = {4,3,3,5,4,4,3,5,5,4};
static const unsigned char teen_len[10]  = {3,6,6,8,8,7,7,9,8,8};
static const unsigned char tens_len[10]  = {0,0,6,6,5,5,5,7,6,6};
static const unsigned char suffix_len[12] = {0,8,7,7,8,10,10,9,9,10,9,10};

/* ------------------------------------------------------------------ */
/* Length of the name of a 0‑to‑999 group.  Uses the American English
   convention that omits a trailing “and”. */
static unsigned group_len(uint16_t grp)
{
    unsigned len = 0;

    if (grp >= 100) {
        unsigned h = grp / 100;
        len += unit_len[h];      /* "three" etc. */
        len += 7;                /* "hundred" */
        grp %= 100;
    }

    if (grp >= 20) {
        unsigned t = grp / 10;
        len += tens_len[t];      /* "twenty" etc. */
        grp %= 10;
    }

    if (grp >= 10 && grp <= 19) {
        len += teen_len[grp - 10];
        grp = 0;
    }

    if (grp > 0 && grp < 10) {
        len += unit_len[grp];
    }

    return len;
}

/* ------------------------------------------------------------------ */
/* Total letter count of any nonnegative integer. */
static unsigned letter_count(uint64_t n)
{
    if (n == 0)
        return unit_len[0];      /* "zero" */

    unsigned total = 0;
    unsigned suffix_idx = 0;

    while (n > 0) {
        uint16_t grp = (uint16_t)(n % 1000);
        if (grp > 0) {
            total += group_len(grp);
            total += suffix_len[suffix_idx];
        }
        n /= 1000;
        ++suffix_idx;
    }

    return total;
}

/* ------------------------------------------------------------------ */
/* Helper structure for sorting. */
struct Item {
    uint64_t n;
    unsigned len;
};

/* Comparison: by n/len, then by n. */
static int cmp_items(const void *pa, const void *pb)
{
    const struct Item *a = (const struct Item *)pa;
    const struct Item *b = (const struct Item *)pb;

    uint64_t lhs = (uint64_t)a->n * b->len;
    uint64_t rhs = (uint64_t)b->n * a->len;

    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    if (a->n < b->n) return -1;
    if (a->n > b->n) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Cached sorted sequence (indices are 0‑based). */
static uint64_t *sorted_cache = NULL;   /* array of numbers */
static size_t   cache_len   = 0;        /* number of elements */

/* One‑time initialization: build the cache up to a fixed limit. */
static void init_sequence(void)
{
    if (sorted_cache != NULL)
        return;   /* already initialized */

    /* We choose a bound that is comfortably larger than needed for
       up to 10 000 terms. */
    const size_t M = 200000;    /* inclusive upper bound */

    struct Item *arr = (struct Item *)malloc((M + 1) * sizeof(struct Item));
    if (!arr)
        exit(1);  /* fatal error */

    for (size_t i = 0; i <= M; ++i) {
        arr[i].n   = (uint64_t)i;
        arr[i].len = letter_count((uint64_t)i);
    }

    qsort(arr, M + 1, sizeof(struct Item), cmp_items);

    sorted_cache = (uint64_t *)malloc((M + 1) * sizeof(uint64_t));
    if (!sorted_cache)
        exit(1);

    for (size_t i = 0; i <= M; ++i)
        sorted_cache[i] = arr[i].n;

    free(arr);
    cache_len = M + 1;
}

/* ------------------------------------------------------------------ */
/* The sequence function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 */
        return (seq_t)0;

    init_sequence();

    /* The build limit is large enough for the required indices. */
    if ((size_t)n > cache_len) {
        return (seq_t)0;
    }

    return (seq_t)sorted_cache[n - 1];
}
