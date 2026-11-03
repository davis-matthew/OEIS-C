/*
 * A251938  Number of length 4+2 0..n arrays with the sum of the maximum minus
 *          the median of adjacent triples multiplied by some arrangement of
 *          +-1 equal to zero.
 * https://oeis.org/A251938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +3*a(n-3) -a(n-4) -2*a(n-5) -3*a(n-6) -3*a(n-7) *
 *            +5*a(n-8) +2*a(n-9) +5*a(n-10) -3*a(n-11) -3*a(n-12) *
 *            -2*a(n-13) -a(n-14) +3*a(n-15) +a(n-17) -a(n-18) * Initial
 *            values a(1)=34, a(2)=339, a(3)=1760, a(4)=6293, * a(5)=17598,
 *            a(6)=41677, a(7)=87328, a(8)=166677, a(9)=295898, * a(10)...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 61, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 91 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper functions for the first 18 terms (brute force). */
static inline idx_t max3(idx_t a, idx_t b, idx_t c) {
    idx_t m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

static inline idx_t min3(idx_t a, idx_t b, idx_t c) {
    idx_t m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

static inline idx_t diff(idx_t x, idx_t y, idx_t z) {
    idx_t mx = max3(x, y, z);
    idx_t mn = min3(x, y, z);
    idx_t sum = x + y + z;
    idx_t med = sum - mx - mn;
    return mx - med;
}

static inline bool has_partition(idx_t d0, idx_t d1, idx_t d2, idx_t d3) {
    idx_t S = d0 + d1 + d2 + d3;
    if (S & 1ULL) return false;
    idx_t target = S >> 1;
    idx_t d[4] = {d0, d1, d2, d3};
    for (idx_t mask = 0; mask < 16; ++mask) {
        idx_t sum = 0;
        if (mask & 1) sum += d[0];
        if (mask & 2) sum += d[1];
        if (mask & 4) sum += d[2];
        if (mask & 8) sum += d[3];
        if (sum == target) return true;
    }
    return false;
}

static unsigned long long count_arrays(idx_t n) {
    unsigned long long cnt = 0;
    for (idx_t a0 = 0; a0 <= n; ++a0)
    for (idx_t a1 = 0; a1 <= n; ++a1)
    for (idx_t a2 = 0; a2 <= n; ++a2)
    for (idx_t a3 = 0; a3 <= n; ++a3)
    for (idx_t a4 = 0; a4 <= n; ++a4)
    for (idx_t a5 = 0; a5 <= n; ++a5) {
        idx_t d0 = diff(a0, a1, a2);
        idx_t d1 = diff(a1, a2, a3);
        idx_t d2 = diff(a2, a3, a4);
        idx_t d3 = diff(a3, a4, a5);
        if (has_partition(d0, d1, d2, d3)) ++cnt;
    }
    return cnt;
}

static void init_first_terms(seq_t *cache) {
    static bool done = false;
    if (done) return;
    cache[0] = (seq_t)0;           /* undefined but set to 0 */
    for (idx_t n = 1; n <= 18; ++n) {
        cache[n] = (seq_t)count_arrays(n);
    }
    done = true;
}

/* Main function computing the sequence. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t allocated = 0;
    static idx_t current = 0;

    /* First call: allocate cache and compute first 18 terms. */
    if (!cache) {
        allocated = 64;
        cache = mem_new(seq_t, allocated);
        if (!cache) return (seq_t)0;
        init_first_terms(cache);
        current = 18;
    }
    if (n == 0) return (seq_t)0;   /* undefined but guard */

    /* Grow cache if necessary. */
    if (n > current) {
        if ((size_t)(n + 1) > allocated) {
            size_t new_cap = allocated * 2;
            if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return (seq_t)0;
            cache = tmp;
            allocated = new_cap;
        }
        /* Compute missing terms using the recurrence. */
        for (idx_t k = current + 1; k <= n; ++k) {
            seq_t sum = cache[k-1];
            sum += cache[k-3] * (seq_t)3;
            sum -= cache[k-4];
            sum -= (seq_t)2 * cache[k-5];
            sum -= (seq_t)3 * cache[k-6];
            sum -= (seq_t)3 * cache[k-7];
            sum += (seq_t)5 * cache[k-8];
            sum += (seq_t)2 * cache[k-9];
            sum += (seq_t)5 * cache[k-10];
            sum -= (seq_t)3 * cache[k-11];
            sum -= (seq_t)3 * cache[k-12];
            sum -= (seq_t)2 * cache[k-13];
            sum -= cache[k-14];
            sum += (seq_t)3 * cache[k-15];
            sum += cache[k-17];
            sum -= cache[k-18];
            cache[k] = sum;
        }
        current = n;
    }
    return cache[n];
}
