/*
 * A249045  Self-numbers (A003052) that are multiples of 3.
 * https://oeis.org/A249045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer that is a self-number (cannot
 *            be * written as m + sum_of_digits(m)) and is divisible by 3. The
 *            function * generates terms on demand and memoises them
 * Range:     u8 n <= 10, u16 n <= 2138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 8117, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *self_terms = NULL;  /* memoised self‑numbers (multiples of 3) */
static size_t  self_cap   = 0;     /* capacity of self_terms */
static size_t  self_cnt   = 0;     /* number of stored terms */

/* number of decimal digits of an idx_t value */
static inline int idx_digits(idx_t n)
{
    int d = 0;
    do {
        d++;
        n /= 10;
    } while (n);
    return d;
}

/* sum of decimal digits of an idx_t value */
static inline uint32_t digit_sum(idx_t x)
{
    uint32_t sum = 0;
    while (x) {
        sum += (uint32_t)(x % 10);
        x /= 10;
    }
    return sum;
}

/* check whether 'x' is a self-number (no m < x satisfies m + sumdigits(m) = x) */
static bool is_self(idx_t x)
{
    int d = idx_digits(x);
    idx_t start = (x > (idx_t)(9u * d)) ? (x - (idx_t)(9u * d)) : 1;
    for (idx_t m = start; m < x; ++m) {
        if (m + (idx_t)digit_sum(m) == x)
            return false;
    }
    return true;
}

/* ensure at least 'n' self-numbers are memoised */
static void ensure_terms(size_t n)
{
    if (self_cnt >= n)
        return;
    /* grow memoisation array if needed */
    if (self_cap == 0) {
        self_cap = 128;
        self_terms = (seq_t *)malloc(self_cap * sizeof(seq_t));
    } else if (self_cnt == self_cap) {
        self_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(self_terms, self_cap * sizeof(seq_t));
        if (!tmp) abort();
        self_terms = tmp;
    }

    /* start searching from the next unexamined candidate */
    static idx_t candidate = 3;         /* lowest multiple of 3 */
    while (self_cnt < n) {
        /* find next multiple of 3 that is a self-number */
        while (!(candidate % 3 == 0) || !is_self(candidate))
            candidate++;

        self_terms[self_cnt++] = (seq_t)candidate;
        candidate += 3;                  /* jump to next multiple of 3 */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* value below offset is undefined */
    ensure_terms((size_t)n);
    return self_terms[n - 1];
}
