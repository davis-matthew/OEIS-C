/*
 * A393581  Numbers k such that the sum of the digits of k raised to the power
 *          of their respective positions has unique digits in strictly
 *          ascending order.
 * https://oeis.org/A393581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 1764, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the smallest k such that the sum of
 * the digits of k raised to their positions has all decimal digits
 * distinct and in strictly ascending order.  The function examines
 * successive integers, computes the required sum, and tests the
 * digits for the ascending property.  It caches found terms so that
 * calls to a(idx_t n) are O(1) after initial work. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

enum { MAX_DIGITS = 20 };                /* enough for 64‑bit indices */

/* pre‑computed powers d^e for d=0..9, e=1..MAX_DIGITS */
static seq_t pow_arr[10][MAX_DIGITS + 1];
static bool pow_ready = false;

static void init_pow(void)
{
    if (pow_ready) return;
    for (int d = 0; d <= 9; ++d) {
        pow_arr[d][0] = (seq_t)1;        /* unused, but harmless */
        for (int e = 1; e <= MAX_DIGITS; ++e) {
            if (d == 0)
                pow_arr[d][e] = (seq_t)0;
            else
                pow_arr[d][e] = pow_arr[d][e-1] * (seq_t)d;
        }
    }
    pow_ready = true;
}

/* Returns true if the decimal digits of sum are all distinct and
 * appear in strictly increasing order from most to least significant. */
static bool ascending_sum(seq_t sum)
{
    if (sum == (seq_t)0) return true;   /* trivial case */

    uint8_t rev[MAX_DIGITS];            /* store digits reversed */
    int len = 0;
    seq_t tmp = sum;
    while (tmp > (seq_t)0) {
        rev[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    /* check strictly increasing from most significant to least */
    for (int i = len - 1; i > 0; --i) {
        if (rev[i - 1] <= rev[i]) return false;
    }
    return true;
}

/* Checks whether a candidate integer n satisfies the sequence criterion. */
static bool qualifies(idx_t n)
{
    init_pow();

    /* extract digits of n in reverse order (LSD first) */
    idx_t temp = n;
    uint8_t rev[MAX_DIGITS];
    int len = 0;
    if (temp == 0) {
        rev[0] = 0;
        len = 1;
    } else {
        while (temp > 0 && len < MAX_DIGITS) {
            rev[len++] = (uint8_t)(temp % 10);
            temp /= 10;
        }
    }

    seq_t sum = (seq_t)0;
    for (int pos = 1; pos <= len; ++pos) {
        uint8_t digit = rev[len - pos]; /* most significant digit at position 1 */
        sum += pow_arr[digit][pos];
    }
    return ascending_sum(sum);
}

/* --- Dynamic cache of discovered terms --- */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* generate terms until we reach the requested index */
    while (cache_len < n) {
        while (!qualifies(next_candidate))
            ++next_candidate;
        ensure_cache_cap(cache_len + 1);
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}
