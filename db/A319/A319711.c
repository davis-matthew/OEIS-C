/*
 * A319711  Sum of A034968(d) over proper divisors d of n, where A034968 gives
 *          the sum of digits in factorial base.
 * https://oeis.org/A319711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n, d<n} A034968(d) where A034968(d) = sum of
 *            digits of d in factorial base
 * Range:     u8 n <= 6299, u16 n <= 9876, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute factorial base digit sum for n. */
static seq_t factorial_digit_sum(idx_t n)
{
    if (n <= 1) return 1;        /* A034968(1) = 1, and 0 never appears as a divisor */

    /* Use 128‑bit arithmetic for the factorial steps to avoid overflow. */
    unsigned __int128 remaining = n;
    unsigned __int128 fact = 1;
    int k = 1;

    /* Find the highest factorial <= n. */
    while (1) {
        unsigned __int128 next = fact * (unsigned __int128)(k + 1);
        if (next > remaining) break;
        fact = next;
        k++;
    }

    seq_t sum = 0;
    while (k > 0) {
        unsigned __int128 d = remaining / fact;
        sum += (seq_t)d;
        remaining -= d * fact;
        if (k == 1) break;
        fact = fact / (unsigned __int128)k;   /* move to (k‑1)! */
        k--;
    }
    return sum;
}

/* Cache for factorial digit sums to speed repeated queries. */
static seq_t *fac_cache = NULL;
static idx_t fac_cache_cap = 0;

static void ensure_fac_cache(idx_t n)
{
    if (n < fac_cache_cap) return;
    idx_t new_cap = fac_cache_cap ? fac_cache_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;
    fac_cache = (seq_t*)realloc(fac_cache, (size_t)new_cap * sizeof(seq_t));
    for (idx_t i = fac_cache_cap; i < new_cap; ++i) fac_cache[i] = 0;
    fac_cache_cap = new_cap;
}

static seq_t cached_fact_digit_sum(idx_t n)
{
    if (n == 0) return 0;
    ensure_fac_cache(n);
    if (fac_cache[n] != 0) return fac_cache[n];
    fac_cache[n] = factorial_digit_sum(n);
    return fac_cache[n];
}

/* Integer square root (floored) using 128‑bit multiplication. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;  /* a(1) = 0, and n == 0 is undefined for OEIS */

    seq_t total = 0;
    idx_t root = isqrt_idx(n);

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            /* divisor d itself */
            if (d != n) total += cached_fact_digit_sum(d);

            /* paired divisor n/d, avoid duplicate when d == n/d */
            idx_t q = n / d;
            if (q != d && q != n) total += cached_fact_digit_sum(q);
        }
    }
    return total;
}
