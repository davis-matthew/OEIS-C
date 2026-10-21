/*
 * A253594  Numbers n that have more than one palindromic representation in
 *          bases 2 <= b <= n-2.
 * https://oeis.org/A253594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_pal_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements OEIS A253594:
 *  a(n) is the n‑th positive integer that has more than one palindromic
 *  representation in bases 2 ≤ b ≤ N‑2 (base N‑1 is excluded, because
 *  every N>2 is written “11” there).  The function searches integers
 *  sequentially and caches the results, so any order of calls is
 *  handled efficiently.  This implementation works for both builtin
 *  unsigned integer types and arbitrary‑precision seq_t classes. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_pal_base.h"

/* Integer square‑root: floor(sqrt(x)) for any unsigned seq_t. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t sq = mid * mid;
        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Return true if 'num' has at least two admissible palindromic
 * representations. */
static bool has_two_palindromes(seq_t num)
{
    size_t cnt = 0;
    seq_t sqrt_num = isqrt_seq(num);
    idx_t sqrt_lim = (idx_t)sqrt_num;

    for (idx_t b = 2; b <= sqrt_lim; ++b) {
        if (is_pal_base(num, b)) {
            if (++cnt >= 2)
                return true;
        }
    }

    /* Two‑digit palindrome: num = a*(b+1), with 1 ≤ a < b. */
    for (idx_t a = 1; a <= sqrt_lim; ++a) {
        if (num % (seq_t)a == 0) {
            seq_t b = num / (seq_t)a - 1;
            if (b > sqrt_lim && b >= 2 && b <= num - 2 &&
                (seq_t)a < b && b != num - 1) {
                if (++cnt >= 2)
                    return true;
            }
        }
    }
    return false;
}

/* Cache of found terms. */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;     /* number of terms stored */
static size_t seq_cap   = 0;     /* allocated capacity */
static seq_t last_checked = 0;   /* last integer examined */

static void grow_cache(size_t needed)
{
    if (needed <= seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < needed)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (seq_cache) {
        for (size_t i = 0; i < seq_len; ++i)
            tmp[i] = seq_cache[i];
        delete [] seq_cache;
    }
    seq_cache = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq_cache = tmp;
#endif
    seq_cap = new_cap;
}

/*  Public entry point required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (seq_len < n) {
        grow_cache(n);
        while (seq_len < n) {
            ++last_checked;
            if (has_two_palindromes(last_checked))
                seq_cache[seq_len++] = last_checked;
        }
    }
    return seq_cache[n - 1];
}
