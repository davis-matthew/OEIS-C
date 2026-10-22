/*
 * A395718  Numbers admitted by the golden exclusion sieve.
 * https://oeis.org/A395718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A395718 – Numbers admitted by the golden exclusion sieve.
   Value a(n) is returned for 1‑based index n.
   The algorithm generates the sieve by iterating over v, computing
   floor(v/φ) with integer arithmetic only, and storing admitted values.
   It keeps a static cache so that requests for any order of indices run in O(n). */

#include <stdlib.h>
#include <stdint.h>

/* integer square root of a 128‑bit integer: floor(sqrt(x)). */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = ((uint64_t)1 << 63) - 1;
    uint64_t ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* floor(v / φ) where φ = (1 + sqrt(5)) / 2, computed without floating point. */
static inline idx_t floor_div_phi(idx_t v)
{
    if (v == 0) return 0;
    unsigned __int128 two_v = (unsigned __int128)v * 2;
    unsigned __int128 val = (unsigned __int128)5 * two_v * two_v;   /* 5 * (2v)² */
    uint64_t sqrt_val = isqrt_uint128(val);                         /* floor(2v * sqrt(5)) */
    int64_t C = (int64_t)sqrt_val - (int64_t)(2 * v);               /* floor((2v)(sqrt5-1)) */
    return (idx_t)(C / 4);                                         /* floor(v / φ) */
}

/* Return the n‑th term of A395718 (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for zero index */

    /* ---------- static cache ---------- */
    static idx_t max_generated_n = 0;   /* number of terms stored in a_arr */
    static size_t a_cap = 0;            /* capacity of a_arr           */
    static seq_t *a_arr = NULL;         /* array of admitted numbers   */

    static idx_t current_v = 0;         /* last processed v           */
    static size_t b_cap = 0;            /* capacity for admitted flag  */
    static uint8_t *b_arr = NULL;       /* b_arr[v] – 1 if v admitted  */
    static idx_t *prefix = NULL;        /* prefix sums of admitted nums */

    /* ---------- first call initialisation ---------- */
    if (a_arr == NULL) {
        a_cap = 1024;
        a_arr = (seq_t *)malloc(sizeof(seq_t) * a_cap);

        b_cap = 1024;
        b_arr = (uint8_t *)malloc(b_cap * sizeof(uint8_t));
        prefix = (idx_t *)malloc(b_cap * sizeof(idx_t));
        prefix[0] = 0;
    }

    /* ---------- generate sequence until a(n) is available ---------- */
    while (max_generated_n < n) {
        /* Grow storage for the next v if needed. */
        if (current_v + 1 >= b_cap) {
            size_t new_cap = b_cap * 2;
            b_arr = (uint8_t *)realloc(b_arr, new_cap * sizeof(uint8_t));
            prefix = (idx_t *)realloc(prefix, new_cap * sizeof(idx_t));
            b_cap = new_cap;
        }

        /* Next integer to examine. */
        ++current_v;
        idx_t v = current_v;

        /* compute m = floor(v/φ) and L = floor(v/φ²). */
        idx_t m = floor_div_phi(v);
        idx_t L = v - m - 1;  /* because floor(v/φ) + floor(v/φ²) = v – 1 */

        /* count of admitted numbers in (m , v) : c = prefix[v-1] - prefix[m]. */
        idx_t c = prefix[v - 1] - prefix[m];
        idx_t rhs = floor_div_phi(L);  /* floor(L/φ) */

        uint8_t admitted = (c <= rhs) ? 1 : 0;
        b_arr[v] = admitted;
        prefix[v] = prefix[v - 1] + admitted;

        if (admitted) {
            /* Grow a_arr if necessary. */
            if (max_generated_n >= a_cap) {
                size_t new_a_cap = a_cap * 2;
                a_arr = (seq_t *)realloc(a_arr, new_a_cap * sizeof(seq_t));
                a_cap = new_a_cap;
            }
            a_arr[max_generated_n] = (seq_t)v;
            ++max_generated_n;
        }
    }

    return a_arr[n - 1];
}
