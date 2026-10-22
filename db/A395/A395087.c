/*
 * A395087  a(n) is the number of vertices of the convex hull of the finite
 *          point set {(k, prime(k)), k = 1..n} (where prime(k) denotes the
 *          k-th prime number).
 * https://oeis.org/A395087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of vertices of the convex hull of {(k, prime(k)),
 *            k = 1..n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"


static idx_t generated_n = 0;
static uint32_t *prime_list = NULL;
static size_t prime_cap = 0;
static idx_t last_candidate = 1;

static void ensure_primes(idx_t n)
{
    while (generated_n < n) {
        last_candidate++;
        if (is_prime(last_candidate)) {
            if (generated_n >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                while (new_cap <= (size_t)generated_n) new_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[generated_n++] = (uint32_t)last_candidate;
        }
    }
}

static idx_t upper_cap = 0, lower_cap = 0;
static idx_t *upper_arr = NULL;
static idx_t *lower_arr = NULL;

static void ensure_hull_cap(idx_t n)
{
    if (n <= upper_cap) return;
    size_t new_cap = upper_cap ? upper_cap * 2 : 4;
    while (new_cap < (size_t)n) new_cap *= 2;

    idx_t *tmp = (idx_t *)realloc(upper_arr, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    upper_arr = tmp;

    tmp = (idx_t *)realloc(lower_arr, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    lower_arr = tmp;

    upper_cap = lower_cap = new_cap;
}

static long long cross_value(idx_t i1, idx_t i2, idx_t ik)
{
    uint32_t y1 = prime_list[(size_t)i1 - 1];
    uint32_t y2 = prime_list[(size_t)i2 - 1];
    uint32_t yk = prime_list[(size_t)ik - 1];
    long long dx1 = (long long)i2 - (long long)i1;
    long long dy1 = (long long)y2 - (long long)y1;
    long long dx2 = (long long)ik - (long long)i1;
    long long dy2 = (long long)yk - (long long)y1;
    return dx1 * dy2 - dy1 * dx2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return (seq_t)1;

    ensure_primes(n);
    ensure_hull_cap(n);

    idx_t upper_sz = 0, lower_sz = 0;

    for (idx_t k = 1; k <= n; k++) {
        /* Upper hull: pop while left turn or collinear */
        while (upper_sz >= 2) {
            idx_t i1 = upper_arr[upper_sz - 2];
            idx_t i2 = upper_arr[upper_sz - 1];
            long long cr = cross_value(i1, i2, k);
            if (cr >= 0) upper_sz--;
            else break;
        }
        upper_arr[upper_sz++] = k;

        /* Lower hull: pop while right turn or collinear */
        while (lower_sz >= 2) {
            idx_t i1 = lower_arr[lower_sz - 2];
            idx_t i2 = lower_arr[lower_sz - 1];
            long long cr = cross_value(i1, i2, k);
            if (cr <= 0) lower_sz--;
            else break;
        }
        lower_arr[lower_sz++] = k;
    }

    /* Number of vertices = (lower hull size + upper hull size - 2) */
    return (seq_t)(lower_sz + upper_sz - 2);
}
