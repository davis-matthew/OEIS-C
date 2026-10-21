/*
 * A277605  Numbers not in a Fibonacci sequence starting with two 1-digit
 *          numbers.
 * https://oeis.org/A277605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A277605:
   a(n) = the n-th positive integer that is NOT present in any
   Fibonacci‑like sequence whose first two terms are 1-digit
   integers 1…9.  The implementation uses a simple sieve: for a
   given upper bound U it marks all numbers ≤ U that are reachable
   from any 1…9 seed pair and then collects the missing ones.  The
   bound is grown when more terms are needed, so a(1) is computed
   correctly and the function remains correct for any n ≥ 1.  */

/* Include only standard headers that are available in both C11 and C++17. */
#include <stdlib.h>
#include <stdbool.h>

/* State that keeps the cache of missing numbers.  The array stores
   values as idx_t; the result is cast to seq_t when returned. */
static idx_t *missing_arr = NULL;
static size_t missing_len = 0;   /* number of elements actually stored */
static size_t missing_cap = 0;   /* capacity of missing_arr */
static idx_t  current_limit = 0; /* largest U processed so far */

/* Rebuild the reachable/missing lists up to the new upper bound new_limit.
   All arithmetic that may overflow seq_t is performed in unsigned __int128,
   which is safe for the small values involved. */
static void recompute(idx_t new_limit)
{
    /* reach[i] == 1 iff i is attainable from some (a,b), 1≤a,b≤9 */
    unsigned char *reach = (unsigned char *)calloc((size_t)new_limit + 1,
                                                   sizeof(unsigned char));
    if (!reach)
        abort();   /* allocation failure */

    /* Enumerate all seed pairs (a,b) in 1…9.  For each, generate the sequence
       until it exceeds new_limit. */
    for (int a = 1; a <= 9; ++a) {
        for (int b = 1; b <= 9; ++b) {
            unsigned __int128 cur0 = (unsigned __int128)a;
            unsigned __int128 cur1 = (unsigned __int128)b;

            if (cur0 <= (unsigned __int128)new_limit)
                reach[(size_t)cur0] = 1;
            if (cur1 <= (unsigned __int128)new_limit)
                reach[(size_t)cur1] = 1;

            while (1) {
                unsigned __int128 nxt = cur0 + cur1;
                if (nxt > (unsigned __int128)new_limit)
                    break;
                reach[(size_t)nxt] = 1;
                cur0 = cur1;
                cur1 = nxt;
            }
        }
    }

    /* Count how many numbers are missing to allocate the array in one go. */
    size_t cnt = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        if (!reach[(size_t)i])
            ++cnt;

    /* Allocate space for the missing numbers. */
    idx_t *new_arr = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!new_arr)
        abort();

    size_t idx = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        if (!reach[(size_t)i])
            new_arr[idx++] = i;

    /* Replace the old array. */
    free(missing_arr);
    missing_arr = new_arr;
    missing_len = cnt;
    missing_cap = cnt;

    /* Clean up. */
    free(reach);
    current_limit = new_limit;
}

/* Ensure that the cached array contains at least n missing numbers.
   If not, grow the bound and recompute. */
static void ensure_missing(idx_t n)
{
    /* Fast path: already have enough. */
    if (missing_len >= n)
        return;

    /* Desired array size with a small safety margin. */
    idx_t target = n + 100;

    /* Start from a reasonable bound if not already initialized. */
    idx_t new_limit = current_limit ? current_limit : 2000;

    /* Grow the bound until it is large enough to cover the target
       number of missing values.  We multiply by two each time to keep
       the number of recomputations logarithmic. */
    while (new_limit < target)
        new_limit = new_limit * 2 + 1000;

    recompute(new_limit);

    /* (In the unlikely case we still haven't got enough, grow further.) */
    while (missing_len < n) {
        new_limit = new_limit * 2 + 1000;
        recompute(new_limit);
    }
}

/* Sequence function.  Undefined for n==0, but callers should only use
   n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_missing(n);
    return (seq_t)missing_arr[n - 1];
}
