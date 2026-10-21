/*
 * A277141  Lexicographically least sequence of nonnegative integers that
 *          avoids a/b-powers for all a/b >= 4/3.
 * https://oeis.org/A277141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function generates the lexicographically least sequence of
 * non‑negative integers that avoids any a/b‑power with a/b ≥ 4/3.
 * It works by greedy construction: at each index it appends the
 * smallest integer that does not create a forbidden pattern ending
 * at that position.  Forbidden patterns are detected by checking
 * all overlaps with ratio ≥ 1/3 (i.e. exponent ≥ 4/3).
 *
 * The implementation memoises the sequence in a static dynamic
 * array so that repeated calls with arbitrary indices are fast
 * (O(n) for the first computation up to n).
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for the sequence */
static seq_t *seq  = NULL;   /* array of terms */
static size_t  cap  = 0;     /* current capacity of seq */
static size_t  used = 0;     /* number of terms stored */

/* enlarge seq so that seq[0..used-1] remains valid and
   capacity is at least 'need' */
static void grow_capacity(size_t need)
{
    if (cap >= need)
        return;
    size_t new_cap = cap ? cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();            /* allocation failure */
    seq = tmp;
    cap = new_cap;
}

/* return true iff arr[0..len-1] contains a forbidden
   a/b‑power ending at position len-1 */
static bool has_forbidden(const seq_t *arr, size_t len)
{
    for (size_t t = 1; t < len; ++t)            /* block length t */
    {
        size_t min_p = (t + 2) / 3;             /* ceil(t/3) */
        size_t max_p = t;
        if (len - t < max_p)
            max_p = len - t;                   /* p ≤ len–t */
        if (min_p > max_p)
            continue;

        for (size_t p = min_p; p <= max_p; ++p)    /* overlap length p */
        {
            size_t start = len - t - p;
            size_t i;
            for (i = 0; i < p; ++i)
                if (arr[start + i] != arr[start + t + i])
                    break;
            if (i == p)
                return true;             /* forbidden pattern found */
        }
    }
    return false;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    /* already computed? */
    if ((idx_t)used > n)
        return seq[(size_t)n];

    /* ensure capacity for new terms */
    grow_capacity((size_t)n + 1);

    /* build missing terms */
    for (idx_t idx = (idx_t)used; idx <= n; ++idx)
    {
        size_t len = used + 1;   /* length after adding candidate */

        /* try successive candidates */
        seq_t cand = 0;
        for (;; ++cand)
        {
            seq[used] = cand;                 /* tentative value */
            if (!has_forbidden(seq, len))
                break;
        }

        ++used;
    }

    return seq[(size_t)n];
}
