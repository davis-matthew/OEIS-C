/*
 * A342642  Numbers k such that A342640(k) = 0.
 * https://oeis.org/A342642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 8134, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that every nonnegative integer is a sum of two
   nonnegative integers not in s(k).  The function returns the nth
   element of this sequence, with a(1)=0.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Test whether a given k satisfies the condition.  */
static bool qualifies(seq_t k)
{
    /* k = 0 is always valid.  */
    if (k == 0) {
        return true;
    }

    /* Find the position of the highest set bit (size of s(k)).  */
    seq_t tmp = k;
    idx_t max_exp = 0;          /* number of bits examined  */
    while (tmp) {
        ++max_exp;
        tmp >>= 1;
    }
    if (max_exp == 0) {         /* should not happen for k>0  */
        return true;
    }

    /* Allocate an array for the allowed exponents.  */
    bool *allowed = (bool *)malloc(max_exp * sizeof(bool));
    if (!allowed) {
        abort();                /* allocation failure  */
    }

    /* Initially assume all exponents < max_exp are allowed.  */
    for (idx_t i = 0; i < max_exp; ++i)
        allowed[i] = true;

    /* Mark forbidden exponents (bits that are 1).  */
    tmp = k;
    idx_t pos = 0;
    while (tmp) {
        if (tmp & 1)
            allowed[pos] = false;
        tmp >>= 1;
        ++pos;
    }

    /* For each forbidden exponent e, check that e can be expressed
       as a sum of two allowed exponents.  */
    tmp = k;
    pos = 0;
    while (tmp) {
        if (tmp & 1) {          /* e = pos is forbidden  */
            bool ok = false;
            for (idx_t a = 1; a < pos; ++a) {
                if (allowed[a] && allowed[pos - a]) {
                    ok = true;
                    break;
                }
            }
            if (!ok) {
                free(allowed);
                return false;
            }
        }
        tmp >>= 1;
        ++pos;
    }

    free(allowed);
    return true;
}

/* Returns the nth element of the sequence.  */
seq_t a(idx_t n)
{
    if (n < 1)          /* outside the offset  */
        return (seq_t)0;

    /* Static cache for memoization.  */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;   /* number of elements stored   */
    static size_t cache_cap  = 0;   /* allocated capacity          */
    static seq_t current_k   = 0;   /* next candidate to test      */

    /* Grow the cache until it contains at least n elements.  */
    while (cache_size < n) {
        if (qualifies(current_k)) {
            /* Ensure sufficient capacity.  */
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();        /* allocation failure  */
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = current_k;
        }
        /* Advance to the next candidate.  */
        ++current_k;
    }

    return cache[n - 1];
}
