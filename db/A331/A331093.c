/*
 * A331093  Numbers such that the sum of their divisors, excluding 1 and the
 *          number itself, minus the sum of their digits equals the number.
 * https://oeis.org/A331093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   numbers n such that σ(n) − 1 − n − digit_sum(n) = n, i.e. σ(n) =
 *            2·n + 1 + digit_sum(n). The sequence consists of a(1) = 12 a(2)
 *            = 114256 a(k) = 28·p with p prime and digit_sum(28·p) = 55 (k ≥
 *            3) in increasing order
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t n); */
#include "../utils/digit_sum.h"    /* seq_t digit_sum(seq_t v); */

static seq_t *seq_cache    = NULL; /* array of computed terms               */
static size_t cache_len    = 0;     /* number of terms stored                 */
static size_t cache_cap    = 0;     /* current capacity of seq_cache          */
static idx_t  next_prime   = 3;     /* next candidate odd number to test     */

static void grow_cache(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_ptr) {
            /* allocation failed – terminate */
            abort();
        }
        seq_cache = new_ptr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* 1‑based indexing of the OEIS sequence */
    while (cache_len < (size_t)n) {
        if (cache_len == 0) {
            /* first term: 12 */
            grow_cache();
            seq_cache[cache_len++] = (seq_t)12;
            continue;
        }
        if (cache_len == 1) {
            /* second term: 114256 */
            grow_cache();
            seq_cache[cache_len++] = (seq_t)114256;
            continue;
        }

        /* generate next term of the form 28·p (p prime) with digit sum 55 */
        while (1) {
            if (is_prime(next_prime)) {
                seq_t val = (seq_t)28 * (seq_t)next_prime;
                if (digit_sum(val) == (seq_t)55) {
                    grow_cache();
                    seq_cache[cache_len++] = val;
                    next_prime += 2; /* advance to next odd candidate */
                    break;           /* go back to outer while to check n */
                }
            }
            next_prime += 2; /* skip even numbers */
        }
    }
    return seq_cache[(size_t)n - 1];
}
