/*
 * A370362  Numbers k such that any two consecutive decimal digits of k^2
 *          differ by 1 after arranging the digits in decreasing order.
 * https://oeis.org/A370362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 115, u32 n <= 160, u64 n <= 160,
 *            u128 n <= 160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate all k ≤ 100 000 such that k² has distinct consecutive digits
   when its digits are sorted in decreasing order.  The sequence is finite
   (160 terms). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *terms = NULL;
static size_t term_count = 0;
static bool built = false;
static const size_t INITIAL_CAP = 200;

/* Build the list of all qualifying k.  Three‑fullpasses in a single function
   keep the code compact and fast enough for all builds. */
static void build_terms(void)
{
    size_t cap = INITIAL_CAP;
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    term_count = 0;

    /* upper bound for k: sqrt(9 999 999 999) < 100 000 */
    for (uint64_t k = 0; k <= 99999; ++k)
    {
        uint64_t sq = k * k;
        bool seen[10] = {false};
        int  min_d = 10, max_d = -1;
        bool dup   = false;

        if (sq == 0)
        {
            seen[0] = true;
            min_d = max_d = 0;
        }
        else
        {
            uint64_t t = sq;
            while (t > 0)
            {
                int d = t % 10;
                if (seen[d])
                {
                    dup = true;
                    break;
                }
                seen[d] = true;
                if (d < min_d) min_d = d;
                if (d > max_d) max_d = d;
                t /= 10;
            }
        }

        if (dup) continue;

        /* count distinct digits */
        int count = 0;
        for (int d = 0; d < 10; ++d)
            if (seen[d]) ++count;

        /* consecutive?   max - min + 1 must equal the number of distinct digits */
        if (max_d - min_d + 1 != count) continue;

        /* store the qualifying k */
        if (term_count == cap)
        {
            cap *= 2;
            terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
        }
        terms[term_count++] = (seq_t)k;
    }
}

seq_t a(idx_t n)
{
    if (!built)
    {
        build_terms();
        built = true;
    }

    if (n == 0)
        return 0;

    size_t idx = (size_t)(n - 1);
    if (idx < term_count)
        return terms[idx];
    return 0; /* out of bounds – safe default */
}
