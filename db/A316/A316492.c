/*
 * A316492  Numbers k such that the average digit in the concatenation of the
 *          numbers from 1 through k is an integer.
 * https://oeis.org/A316492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer k such that the average digit of the
   concatenation of the numbers from 1 through k is an integer.  The sequence
   stops at k = 4576; all terms beyond that do not satisfy the condition.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper to compute the number of decimal digits of an idx_t value.  */
static uint64_t digit_len_u64(idx_t x)
{
    uint64_t len = 0;
    if (x == 0) return 1;
    while (x) {
        ++len;
        x /= 10;
    }
    return len;
}

/* Helper to compute the sum of decimal digits of an idx_t value.  */
static uint64_t digit_sum_u64(idx_t x)
{
    uint64_t sum = 0;
    while (x) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* Ensure that *arr has room for at least 'needed' entries.  */
static void ensure_capacity(size_t *cap, seq_t **arr, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = tmp;
    *cap = new_cap;
}

/* ----- Global static state used by the sequence routine ----- */
#define MAX_K 5000                     /* upper bound needed to discover all terms */
static seq_t  *cache      = NULL;     /* array of found terms      */
static size_t cache_len   = 0;        /* number of terms stored    */
static size_t cache_cap   = 0;        /* capacity of the array     */
static idx_t  next_k      = 1;        /* next value of k to test   */
static uint64_t cum_sum   = 0;        /* cumulative sum of digits  */
static uint64_t cum_digits= 0;        /* cumulative digit count    */

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for index 0 */

    /* Generate terms on demand until we have at least n terms
       or until we have exhausted the known range. */
    while (cache_len < n && next_k <= MAX_K) {
        uint64_t len_k = digit_len_u64(next_k);
        uint64_t sum_k = digit_sum_u64(next_k);
        cum_sum   += sum_k;
        cum_digits+= len_k;

        if (cum_sum % cum_digits == 0) {
            ensure_capacity(&cache_cap, &cache, cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return (n <= cache_len) ? cache[n-1] : 0;
}
