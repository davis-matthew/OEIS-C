/*
 * A250090  a(0)=1; for n>0, a(n) is the number not yet used which yields the
 *          smallest possible positive sum_{k=0..n} (-1)^(a(k)+1)*a(k).
 * https://oeis.org/A250090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n > 0 choose the smallest unused non‑negative
 *            integer that yields the minimal value of the alternating sum
 *            s(n) = Σ_{k=0..n} (-1)^(a(k)+1) * a(k). The greedy rule can be
 *            implemented as: if an unused even number smaller than the
 *            current sum exists, take the largest such even; otherwi...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Track which integers have already been used: 0 = unused, 1 = used. */
static unsigned char *used = NULL;
static size_t          used_cap = 0;

/* Ensure that the used array can hold indices up to (needed-1). */
static void ensure_used_cap(size_t needed)
{
    if (needed <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 128;
    if (new_cap < needed)
        new_cap = needed;
    unsigned char *tmp = (unsigned char*)realloc(used, new_cap);
    if (!tmp && new_cap > 0)
        exit(EXIT_FAILURE);
    if (new_cap > used_cap)
        memset(tmp + used_cap, 0, new_cap - used_cap);
    used     = tmp;
    used_cap = new_cap;
}

/* Cache for the sequence values. */
static seq_t            *seq_cache = NULL;
static size_t            seq_cap   = 0;
static idx_t             current_len = 0;   /* next index to compute */
static seq_t             current_sum = 0;   /* alternating sum so far */

/* Ensure that the sequence cache can hold indices up to (need-1). */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        exit(EXIT_FAILURE);
    seq_cache = tmp;
    seq_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Make sure the cache is large enough. */
    size_t need_len = (size_t)n + 1;
    ensure_seq_cap(need_len);

    /* Compute terms up to the requested index (memoized). */
    while (current_len <= n)
    {
        if (current_len == 0)            /* a(0) = 1 */
        {
            seq_cache[0] = (seq_t)1;
            ensure_used_cap(2);           /* cover 0 and 1 */
            used[1] = 1;                  /* 1 is used */
            current_sum = (seq_t)1;
            current_len = 1;
            continue;
        }

        /* Current alternating sum before adding the next term. */
        seq_t sum = current_sum;

        /* Search for the largest unused even number < sum. */
        bool  found_even = false;
        seq_t best_even  = 0;
        if (sum > (seq_t)0)
        {
            seq_t max_even = sum - (seq_t)1;        /* strictly < sum */
            seq_t start = max_even;
            if ((start % (seq_t)2) != (seq_t)0)     /* make even */
                start -= (seq_t)1;
            seq_t x = start;
            while (true)
            {
                size_t idx = (size_t)x;
                ensure_used_cap(idx + 1);
                if (!used[idx])
                {
                    best_even = x;
                    found_even = true;
                    break;
                }
                if (x == (seq_t)0)
                    break;
                x -= (seq_t)2;
            }
        }

        if (found_even)         /* choose the even (subtracting). */
        {
            seq_cache[current_len] = best_even;
            size_t idx = (size_t)best_even;
            ensure_used_cap(idx + 1);
            used[idx] = 1;
            current_sum = sum - best_even;
            current_len++;
            continue;
        }

        /* No suitable even: choose the smallest unused odd number. */
        seq_t odd = (seq_t)1;
        while (true)
        {
            size_t idx = (size_t)odd;
            ensure_used_cap(idx + 1);
            if (!used[idx])
                break;
            odd += (seq_t)2;
        }
        seq_cache[current_len] = odd;
        used[(size_t)odd] = 1;
        current_sum = sum + odd;
        current_len++;
    }

    return seq_cache[(size_t)n];
}
