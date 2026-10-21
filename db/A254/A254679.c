/*
 * A254679  Irregular triangle read by rows: row n lists the divisors of n,
 *          ordered lexicographically in decimal representation.
 * https://oeis.org/A254679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1461, u16 n <= 7069, u32 n <= 7069, u64 n <= 7069,
 *            u128 n <= 7069, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A254679: the n‑th term is the divisor whose row (number k)
   is the row containing the n‑th element when each row lists
   the divisors of k in lexicographic (decimal string) order. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* helper: compare two divisor structs lexicographically */
typedef struct { uint64_t val; char str[21]; } Div;
static int cmp_div(const void *p1, const void *p2)
{
    const Div *d1 = (const Div *)p1;
    const Div *d2 = (const Div *)p2;
    return strcmp(d1->str, d2->str);
}

/* helper: count divisors of n */
static uint64_t divisor_count(uint64_t n)
{
    uint64_t cnt = 0;
    for (uint64_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            ++cnt;
            if (i != n / i) ++cnt;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* static state for cumulative sums of divisor counts */
    static idx_t current_row = 0;      /* last processed row number      */
    static idx_t *prefix = NULL;       /* prefix[row] = sum_{i=1}^{row} d(i) */
    static size_t prefix_cap = 0;      /* allocated capacity              */

    /* initialise prefix table */
    if (prefix == NULL)
    {
        prefix_cap = 16;
        prefix = (idx_t *)malloc(prefix_cap * sizeof(idx_t));
        if (!prefix) return 0;         /* out of memory: return 0 as safe fallback */
        prefix[0] = 0;
    }

    /* extend cumulative sums until we cover index n */
    idx_t cum = prefix[current_row];
    while (cum < n)
    {
        ++current_row;
        uint64_t cnt = divisor_count(current_row);
        cum += cnt;
        if (current_row >= prefix_cap)
        {
            size_t new_cap = prefix_cap * 2;
            idx_t *tmp = (idx_t *)realloc(prefix, new_cap * sizeof(idx_t));
            if (!tmp) return 0;
            prefix = tmp;
            prefix_cap = new_cap;
        }
        prefix[current_row] = cum;
    }

    /* determine position within the current row */
    idx_t prev_prefix = (current_row > 0) ? prefix[current_row - 1] : 0;
    idx_t offset = n - prev_prefix;        /* 1‑based position in the row    */
    idx_t pos = offset - 1;                /* 0‑based index in the sorted list */

    /* generate all divisors of current_row and sort lexicographically */
    size_t div_cap = 16;
    size_t div_cnt = 0;
    Div *divs = (Div *)malloc(div_cap * sizeof(Div));
    if (!divs) return 0;
    uint64_t m = current_row;
    for (uint64_t i = 1; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            if (div_cnt >= div_cap) {
                div_cap *= 2;
                Div *tmp = (Div *)realloc(divs, div_cap * sizeof(Div));
                if (!tmp) { free(divs); return 0; }
                divs = tmp;
            }
            snprintf(divs[div_cnt].str, sizeof(divs[div_cnt].str), "%llu",
                     (unsigned long long)i);
            divs[div_cnt].val = i;
            ++div_cnt;

            uint64_t j = m / i;
            if (j != i)
            {
                if (div_cnt >= div_cap) {
                    div_cap *= 2;
                    Div *tmp = (Div *)realloc(divs, div_cap * sizeof(Div));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                snprintf(divs[div_cnt].str, sizeof(divs[div_cnt].str), "%llu",
                         (unsigned long long)j);
                divs[div_cnt].val = j;
                ++div_cnt;
            }
        }
    }

    /* sort divisors */
    qsort(divs, div_cnt, sizeof(Div), cmp_div);

    /* fetch the required divisor */
    seq_t result = (seq_t)divs[pos].val;

    free(divs);
    return result;
}
