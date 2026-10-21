/*
 * A258188  Smallest multiple of 7 not appearing earlier that ends with n.
 * https://oeis.org/A258188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest unused multiple of 7 ending with decimal
 *            representation of n
 * Range:     u8 n <= 10, u16 n <= 1105, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   modinv_7_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/modinv_7_uint64.h"

/* Work around the absence of a supplied comparison routine by
 * providing our own which works for all supported seq_t types.   */
static inline int seq_compare(seq_t a, seq_t b)
{
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Number of decimal digits of an idx_t (idx_t is uint64_t). */
static unsigned int num_digits(idx_t x)
{
    unsigned int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

/* Precomputed 10^m and inv7 mod 10^m for 0 ≤ m ≤ 12.
 * These are enough for all n ≤ 10^12 (which exceeds the needed range). */
static uint64_t pow10_arr[13];
static uint64_t inv7_arr[13];
static bool prep_done = false;
static void prepare_tables(void)
{
    if (prep_done) return;
    pow10_arr[0] = 1ULL;
    for (int i = 1; i <= 12; ++i) pow10_arr[i] = pow10_arr[i-1] * 10ULL;
    for (int m = 1; m <= 12; ++m) inv7_arr[m] = modinv_7_uint64(pow10_arr[m]);
    prep_done = true;
}

/* Working state for memoisation. */
static idx_t   max_computed = 0;      /* number of terms already computed */
static seq_t * seq_cache    = NULL;   /* array of sequence values            */
static size_t seq_cap      = 0;       /* allocated capacity of seq_cache     */

static seq_t * used_vals    = NULL;   /* sorted array of used multiples     */
static size_t used_cap     = 0;       /* allocated capacity of used_vals    */
static size_t used_cnt     = 0;       /* number of used elements             */

/* Binary search in used_vals; returns true if found.
 * On exit *pos is the index of found element or insertion point. */
static bool used_contains(seq_t val, size_t *pos)
{
    size_t lo = 0, hi = used_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        int c = seq_compare(val, used_vals[mid]);
        if (c == 0) {
            *pos = mid;
            return true;
        }
        if (c < 0)
            hi = mid;
        else
            lo = mid + 1;
    }
    *pos = lo;
    return false;
}

/* Insert val into used_vals at position pos (sorted). */
static void used_insert_at(seq_t val, size_t pos)
{
    if (used_cnt == used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 64;
        seq_t * tmp = (seq_t *)realloc(used_vals, new_cap * sizeof(seq_t));
        used_vals = tmp;
        used_cap = new_cap;
    }
    for (size_t i = used_cnt; i > pos; --i) used_vals[i] = used_vals[i-1];
    used_vals[pos] = val;
    ++used_cnt;
}

seq_t a(idx_t n)
{
    if (n < 1)           /* undefined for non‑positive index, return 0 */
        return (seq_t)0;

    prepare_tables();

    /* Compute sequence terms up to n if required (memoisation). */
    if (n > max_computed) {
        if (n > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 64;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t * tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = tmp;
            seq_cap = new_cap;
        }

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            unsigned int d = num_digits(i);
            uint64_t mod  = pow10_arr[d];          /* 10^d */
            uint64_t inv7 = inv7_arr[d];           /* inverse of 7 mod 10^d */
            uint64_t base_k = ((uint64_t)i * inv7) % mod;
            uint64_t incr   = mod;                 /* step to keep same last digits */

            uint64_t k = base_k;
            for (;;) {
                seq_t candidate = (seq_t)k * (seq_t)7;
                size_t pos;
                if (!used_contains(candidate, &pos)) {
                    /* found candidate not used yet */
                    seq_cache[i-1] = candidate;
                    used_insert_at(candidate, pos);
                    break;
                }
                k += incr;                          /* next number with same last d digits */
            }
        }

        max_computed = n;
    }

    return seq_cache[n-1];
}
