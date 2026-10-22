/*
 * A362232  a(1) = 1; for n > 1, a(n) is number of terms in the first n-1
 *          terms of the sequence that are not proper divisors of a(n-1).
 * https://oeis.org/A362232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = (n-1) - (# of a(k) for k < n that
 *            are proper divisors of a(n-1))
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

/* --- global state ---------------------------------------------------- */
static seq_t *seq_arr   = NULL;   /* 1‑indexed array of sequence values */
static size_t seq_cap   = 0;      /* allocated capacity for seq_arr */
static seq_t *freq_arr  = NULL;   /* frequency of each value (0‑indexed) */
static size_t freq_cap  = 0;      /* allocated capacity for freq_arr */
static idx_t  computed_max_n = 0; /* highest n that has been computed */

/* --- helpers ----------------------------------------------------------- */

/* Integer square root of a 64‑bit unsigned integer (floor). */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (n >> 1) + 1;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else         hi = mid;
    }
    return lo - 1;
}

/* Ensure seq_arr can hold indices up to n (1‑indexed). */
static void ensure_seq_cap(idx_t n)
{
    size_t need = (size_t)n + 1;            /* indices 0 .. n */
    if (need <= seq_cap) return;
    size_t new_cap = (seq_cap == 0) ? 2 : seq_cap << 1;
    while (new_cap < need) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure freq_arr can hold indices up to value (0‑indexed). */
static void ensure_freq_cap(uint64_t value)
{
    size_t need = (size_t)value + 1;
    if (need <= freq_cap) return;
    size_t old_cap = freq_cap;
    size_t new_cap = (freq_cap == 0) ? 2 : freq_cap << 1;
    while (new_cap < need) new_cap <<= 1;
    freq_arr = (seq_t *)realloc(freq_arr, new_cap * sizeof(seq_t));
    /* zero newly allocated slots */
    for (size_t i = old_cap; i < new_cap; ++i) freq_arr[i] = 0;
    freq_cap = new_cap;
}

/* --- main function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* out of bounds, not expected */

    /* Compute sequence up to n if necessary. */
    if (computed_max_n < n) {
        /* Initialise first term if needed. */
        if (computed_max_n == 0) {
            ensure_seq_cap(1);
            seq_arr[1] = (seq_t)1;
            ensure_freq_cap(1);
            freq_arr[1] = (seq_t)1;
            computed_max_n = 1;
        }

        /* Build subsequent terms. */
        for (idx_t k = computed_max_n + 1; k <= n; ++k) {
            seq_t prev = seq_arr[k - 1];
            uint64_t v = (uint64_t)prev;      /* value of a(k-1) */

            /* Count occurrences of proper divisors of v in the prefix. */
            size_t divisor_cnt = 0;
            uint64_t sq = isqrt_u64(v);
            for (uint64_t d = 1; d <= sq; ++d) {
                if (v % d == 0) {
                    uint64_t d1 = d;
                    uint64_t d2 = v / d;

                    if (d1 < v) {
                        ensure_freq_cap(d1);
                        divisor_cnt += (size_t)freq_arr[d1];
                    }
                    if (d2 != d1 && d2 < v) {
                        ensure_freq_cap(d2);
                        divisor_cnt += (size_t)freq_arr[d2];
                    }
                }
            }

            seq_t ak = (seq_t)(k - 1) - (seq_t)divisor_cnt;
            ensure_seq_cap(k);
            seq_arr[k] = ak;

            /* Update frequency table for future steps. */
            uint64_t val = (uint64_t)ak;
            ensure_freq_cap(val);
            freq_arr[val] += (seq_t)1;
        }

        computed_max_n = n;
    }

    return seq_arr[n];
}
