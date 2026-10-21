/*
 * A292035  Linear divisibility sequence based on Salem number of order 4
 *          (case t=7, see formula).
 * https://oeis.org/A292035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 7*a(n-2) + 7*a(n-3) - a(n-4) for n>4, a(1)=1,
 *            a(2)=7, a(3)=41, a(4)=245
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static storage for the first 10000 terms; sufficient for n≈10000   */
#define MAX_CACHE 10000
static seq_t cache[MAX_CACHE];
static size_t cache_size = 0;   /* number of terms currently computed */

/* ensure the first four terms are present */
static void init_initial(void) {
    if (cache_size >= 4)
        return;
    cache[0] = 1;   /* a(1) */
    cache[1] = 7;   /* a(2) */
    cache[2] = 41;  /* a(3) */
    cache[3] = 245; /* a(4) */
    cache_size = 4;
}

/* compute terms up to index n (1‑based) */
static void compute_to(idx_t n) {
    if (n <= cache_size)
        return;
    if (n > MAX_CACHE) {
        /* exceeding the static cache size – abort */
        abort();
    }
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        seq_t next = 7 * cache[i - 1];
        next -= 7 * cache[i - 2];
        next += 7 * cache[i - 3];
        next -= cache[i - 4];
        cache[i] = next;
    }
    cache_size = (size_t)n;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* seq is 1‑based; term 0 is undefined */
    init_initial();
    compute_to(n);
    return cache[(size_t)(n - 1)];
}
