/*
 * A374736  a(n) is the least number of the form k*n for some k > 0 that can
 *          be added to n without carries in decimal.
 * https://oeis.org/A374736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = least multiple k*n such that adding it to n produces no
 *            decimal carry
 * Range:     u8 n <= 18, u16 n <= 298, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs a bounded brute‑force search for k (1 ≤ k ≤ 10^d),
   where d is the number of decimal digits of n, which is guaranteed to succeed. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache for previously computed values */
static seq_t *cache = NULL;
static bool  *computed = NULL;
static size_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size)
        return;
    size_t new_sz = (size_t)n + 1;
    cache     = (seq_t *)realloc(cache,     new_sz * sizeof(seq_t));
    computed  = (bool *)realloc(computed,  new_sz * sizeof(bool));
    for (size_t i = cache_size; i < new_sz; ++i)
        computed[i] = false;
    cache_size = new_sz;
}

/* compute A374736(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_cache(n);
    if (computed[n])
        return cache[n];

    /* digits of n (least significant first) */
    unsigned nd[32];
    size_t len_n = 0;
    seq_t tmp_n  = (seq_t)n;
    while (tmp_n > 0) {
        nd[len_n++] = (unsigned)(tmp_n % 10);
        tmp_n /= 10;
    }

    seq_t n_seq = (seq_t)n;

    /* upper bound for k: 10^len_n always works */
    seq_t max_k = 1;
    for (size_t i = 0; i < len_n; ++i) {
        max_k *= 10;
    }

    seq_t k       = 1;
    seq_t result  = 0;

    /* search for the minimal k */
    while (1) {
        seq_t m = k * n_seq;

        seq_t t   = m;
        size_t pos = 0;
        bool ok = true;
        while (t > 0) {
            unsigned m_digit = (unsigned)(t % 10);
            unsigned n_digit = (pos < len_n) ? nd[pos] : 0;
            if (n_digit + m_digit > 9) {
                ok = false;
                break;
            }
            t   /= 10;
            ++pos;
        }

        if (ok) {
            result = m;
            break;
        }

        ++k;
        /* k never exceeds max_k because 10^len_n is a valid multiplier */
    }

    cache[n]    = result;
    computed[n] = true;
    return result;
}
