/*
 * A337082  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with 2*x^2 +
 *          4*y^2 - 7*x*y a power of two (including 2^0 = 1), where x, y, z, w
 *          are nonnegative integers with z <= w.
 * https://oeis.org/A337082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this function computes the OEIS sequence A337082. * a(n) =
 *            number of representations n = x^2 + y^2 + z^2 + w^2 * with
 *            nonnegative integers x,y,z,w, * z ≤ w, and 2*x^2 + 4*y^2 – 7*x*y
 *            is a power of two. * * The implementation recomputes the
 *            required pre‑data for each * call, with a cost O(n). It w...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* integer square root for 64‑bit values (C++ and C compatible) */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t y = x | (1ULL << i);
        if ((unsigned __int128)y * y <= n)
            x = y;
    }
    return x;
}

/* power‑of‑two test (unsigned) */
static inline bool is_power_of_two_u64(uint64_t v)
{
    return v && !(v & (v - 1));
}

/* maximal n expected (adjust if larger values are needed) */
#define MAX_N 10000U

/* temporary storage reused for every call */
static seq_t rep_cache[MAX_N + 1];
typedef struct { uint32_t x; uint32_t y; } xy_pair;
static xy_pair pair_arr[MAX_N + 1];
static size_t pair_total;

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return 0;

    if (n > (idx_t)MAX_N)
        /* assume callers never request larger n; for safety */
        return 0;

    /* ----- Step 1: count z^2 + w^2 pairs ----- */
    const size_t N = (size_t)n;
    memset(rep_cache, 0, (N + 1) * sizeof(seq_t));

    const uint64_t limit = isqrt_u64(N);
    for (uint64_t z = 0; z <= limit; ++z) {
        uint64_t z2 = z * z;
        for (uint64_t w = z; w <= limit; ++w) {
            uint64_t w2 = w * w;
            uint64_t sum = z2 + w2;
            if (sum > N)
                break;
            rep_cache[sum] += 1;
        }
    }

    /* ----- Step 2: collect admissible (x,y) pairs ----- */
    pair_total = 0;
    for (uint64_t x = 0; x <= limit; ++x) {
        uint64_t x2 = x * x;
        for (uint64_t y = 0; y <= limit; ++y) {
            uint64_t y2 = y * y;
            uint64_t sum_xy = x2 + y2;
            if (sum_xy > N)
                continue;

            long long val = (long long)2 * (long long)x2
                          + (long long)4 * (long long)y2
                          - (long long)7 * (long long)x * (long long)y;
            if (val <= 0)
                continue;
            if (!is_power_of_two_u64((uint64_t)val))
                continue;

            if (pair_total < MAX_N + 1) {
                pair_arr[pair_total].x = (uint32_t)x;
                pair_arr[pair_total].y = (uint32_t)y;
                ++pair_total;
            }
        }
    }

    /* ----- Step 3: sum contributions ----- */
    seq_t total = 0;
    for (size_t i = 0; i < pair_total; ++i) {
        uint64_t x = pair_arr[i].x;
        uint64_t y = pair_arr[i].y;
        uint64_t sum_xy = x * x + y * y;
        if (sum_xy > N)
            continue;
        size_t remaining = N - (size_t)sum_xy;
        total += rep_cache[remaining];
    }

    return total;
}
