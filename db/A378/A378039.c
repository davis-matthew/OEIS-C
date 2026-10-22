/*
 * A378039  a(1)=3; a(n>1) = n-th first difference of A120327(k) = least
 *          nonsquarefree number greater than k.
 * https://oeis.org/A378039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3; For n>1: a(n)=A120327(n)-A120327(n-1), where A120327(k)
 *            is the least nonsquarefree integer > k
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Forward declarations of build and query helpers. */
static void build_nsf(idx_t need_limit);
static void ensure_limit(idx_t required);
static uint64_t next_nsf(idx_t k);

/* Static storage for the computed list of nonsquarefree numbers. */
static uint64_t *nsf_array = NULL;   /* list of nonsquarefree numbers */
static size_t     nsf_cnt   = 0;      /* number of elements in nsf_array */
static idx_t      limit_max = 0;      /* largest limit for which we built the sieve */

/* Build the sieve of nonsquarefree numbers up to NEW_LIMIT
   and store all nonsquarefree numbers in nsf_array. */
static void build_nsf(idx_t new_limit)
{
    /* Memory for marking whether a number is squarefree (true) or not. */
    bool *mark = (bool *)malloc((size_t)new_limit + 1);
    if (!mark) exit(1);
    memset(mark, 1, (size_t)new_limit + 1);          /* initially all true (squarefree) */

    /* Mark multiples of squares as nonsquarefree. */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        idx_t sq = i * i;
        for (idx_t j = sq; j <= new_limit; j += sq)
            mark[j] = 0;                            /* 0 => nonsquarefree */
    }

    /* Count nonsquarefree numbers. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!mark[i]) ++cnt;

    /* Allocate and fill the array of nonsquarefree numbers. */
    uint64_t *tmp = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!tmp) exit(1);
    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (!mark[i]) tmp[pos++] = (uint64_t)i;
    }

    /* Replace old array. */
    free(nsf_array);
    nsf_array = tmp;
    nsf_cnt   = cnt;
    limit_max = new_limit;

    free(mark);
}

/* Ensure that we have synthesized data covering all numbers up to REQUIRED. */
static void ensure_limit(idx_t required)
{
    if (required <= limit_max) return;

    /* Grow by a buffer to avoid frequent reallocations. */
    idx_t new_limit = required + 1000;
    if (new_limit <= limit_max) new_limit = limit_max + 1000;
    build_nsf(new_limit);
}

/* Binary search: first nonsquarefree number > k. */
static uint64_t next_nsf(idx_t k)
{
    ensure_limit(k);                       /* make sure data covers k */

    size_t l = 0, r = nsf_cnt;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (nsf_array[m] <= (uint64_t)k)
            l = m + 1;
        else
            r = m;
    }

    /* If we ran out of numbers, enlarge and retry. */
    while (l == nsf_cnt) {
        ensure_limit(limit_max * 2 + 1000);
        l = 0; r = nsf_cnt;
        while (l < r) {
            size_t m = (l + r) >> 1;
            if (nsf_array[m] <= (uint64_t)k)
                l = m + 1;
            else
                r = m;
        }
    }
    return nsf_array[l];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;

    /* Values for n and n-1 are needed. */
    uint64_t val_n   = next_nsf(n);
    uint64_t val_nm1 = next_nsf(n - 1);

    return (seq_t)(val_n - val_nm1);
}
