/*
 * A346290  Numbers k = s * t such that reverse(k) = reverse(s) * reverse(t)
 *          where reverse(k) is k with its digits reversed. A single-digit
 *          number is its own reversal and neither s nor t has a leading zero.
 *          No pair (s, t) has both s and t palindromic or single-digit.
 * https://oeis.org/A346290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: brute‑force generation for OEIS sequence A346290.
 * a(n) is the n‑th integer k (n≥1) that can be factored as k = s * t
 * with s>1, t>1, neither s nor t ending in 0, and satisfying
 * reverse(k) = reverse(s) * reverse(t),
 * where reverse(x) is x with its decimal digits reversed.
 * Additionally, the pair (s,t) must not consist of two palindromic
 * numbers or two single‑digit numbers.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"

/* decimal reversal of an unsigned integer value of type seq_t */

/* check whether a seq_t value is a palindrome in base 10 */
static bool is_palindrome(seq_t x)
{
    return reverse_digits(x) == x;
}

/* test if a candidate k has the required reverse‑multiplication property */
static bool has_property(idx_t k)
{
    seq_t revk = reverse_digits((seq_t)k);

    for (idx_t d = 1; d <= k / d; ++d) {
        if (k % d != 0)
            continue;
        idx_t s = d;
        idx_t t = k / d;

        /* ignore trivial factorization with 1, and factors ending in 0 */
        if (s == 1 || t == 1)
            continue;
        if ((s % 10 == 0) || (t % 10 == 0))
            continue;

        seq_t revs = reverse_digits((seq_t)s);
        seq_t revt = reverse_digits((seq_t)t);

        if (revs == 0 || revt == 0)        /* should not happen, but safe */
            continue;

        if (revk % revs != 0)
            continue;
        if (revk / revs != revt)
            continue;                     /* rev(s)*rev(t) != rev(k) */

        /* reject pairs that are both single‑digit or both palindromic */
        bool s_pal = is_palindrome((seq_t)s);
        bool t_pal = is_palindrome((seq_t)t);
        if (!((s_pal && t_pal) || (s < 10 && t < 10)))
            return true;
    }
    return false;
}

/* memoized generator of the OEIS sequence A346290 */
seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence offset is 1; ignore n==0 */
        return 0;

    static seq_t *cache     = NULL;
    static idx_t cache_len  = 0;
    static idx_t cache_cap  = 0;
    static idx_t last_k     = 1;   /* last tested integer (exclusive) */

    while (cache_len < n) {
        /* search for the next qualifying number */
        for (++last_k; ; ++last_k) {
            if (has_property(last_k)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();   /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)last_k;
                break;
            }
        }
    }
    return cache[n - 1];
}
