/*
 * A303578  List of starts of nondecreasing runs of values of d(n) (the
 *          divisor function A000005(n)).
 * https://oeis.org/A303578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Start positions of non‑decreasing runs of the divisor function d(n). */
/* For n >= 1 the function returns the index at which the nth such run begins. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>          /* for sqrt */

/* Dynamic array of run starts */
static idx_t *starts   = NULL;
static size_t start_len = 0;
static size_t start_cap = 0;

/* Largest n that has been processed */
static idx_t current_n = 0;

/* d(n) of the most recently processed n */
static seq_t prev_d = 0;

/* Integer square root using floating point as a starting estimate */
static inline idx_t isqrt_idx(idx_t n)
{
    if (n < 2) return n;
    idx_t r = (idx_t)sqrt((long double)n);
    while ((r + 1) <= n / (r + 1)) ++r;
    while (r > n / r) --r;
    return r;
}

/* Count the number of divisors of n */
static seq_t divisor_count(idx_t n)
{
    seq_t cnt = 0;
    idx_t r = isqrt_idx(n);
    for (idx_t i = 1; i <= r; ++i) {
        if (n % i == 0) {
            if (i == n / i)  cnt += 1;
            else             cnt += 2;
        }
    }
    return cnt;
}

/* Append a new run start to the dynamic array */
static void add_start(idx_t val)
{
    if (start_len == start_cap) {
        start_cap = start_cap ? start_cap * 2 : 64;
        idx_t *tmp = (idx_t *)realloc(starts, start_cap * sizeof(idx_t));
        starts = tmp;
    }
    starts[start_len++] = val;
}

/* Compute run starts until at least target_len starts are stored */
static void ensure_starts(size_t target_len)
{
    while (start_len < target_len) {
        ++current_n;
        seq_t d = divisor_count(current_n);
        if (current_n == 1 || prev_d > d)
            add_start(current_n);
        prev_d = d;
    }
}

/* Required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1; n>=1 in real use */
    ensure_starts((size_t)n);
    return (seq_t)starts[n - 1];
}
