/*
 * A287654  a(1) = 1, a(2) = a(3) = a(4) = 2; a(n) = a(a(n-1)) + a(a(n-a(n-1))
 *          + a(n-a(n-2))) for n > 4.
 * https://oeis.org/A287654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=a(3)=a(4)=2; for n>4: a(n)=a(a(n-1)) + a(
 *            a(n-a(n-1)) + a(n-a(n-2)) );
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;
static size_t   a_cap   = 0;

static void ensure_cap(idx_t n) {
    if (n <= a_cap) return;
    size_t old_cap = a_cap;
    size_t new_cap = old_cap ? old_cap << 1 : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    /* initialise newly allocated part to zero (0 signals “not computed”) */
    memset(a_cache + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
    a_cap = new_cap;
}

static seq_t get_a(idx_t n) {
    /* Base cases */
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2 || n == (idx_t)3 || n == (idx_t)4) return (seq_t)2;

    /* Cached value? */
    if ((size_t)(n - 1) < a_cap && a_cache[n - 1]) return a_cache[n - 1];

    ensure_cap(n);

    /* a(n-1) */
    seq_t a_n_1 = get_a(n - 1);
    seq_t term1 = get_a((idx_t)a_n_1);                          /* a(a(n-1)) */

    /* a(n - a(n-1)) */
    idx_t idx2 = n - (idx_t)a_n_1;
    seq_t a_idx2 = get_a(idx2);

    /* a(n - a(n-2)) */
    seq_t a_n_2 = get_a(n - 2);
    idx_t idx3 = n - (idx_t)a_n_2;
    seq_t a_idx3 = get_a(idx3);

    /* sum = a(n-a(n-1)) + a(n-a(n-2)) */
    seq_t sum = a_idx2 + a_idx3;
    seq_t term2 = get_a((idx_t)sum);           /* a(sum) */

    seq_t res = term1 + term2;
    a_cache[n - 1] = res;
    return res;
}

seq_t a(idx_t n) {
    return get_a(n);
}
