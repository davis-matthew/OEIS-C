/*
 * A294097  Number of partitions of 2n into two parts such that both parts are
 *          either squarefree or nonsquarefree.
 * https://oeis.org/A294097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} [A008966(k) = A008966(2n-k)]
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of 2n into two parts where
   both parts are either squarefree or nonsquarefree. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* integer floor sqrt for size_t */
static size_t int_sqrt(size_t n) {
    size_t left = 0, right = n;
    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        if (mid == 0) { left = mid + 1; continue; }
        if (mid <= n / mid) {
            left = mid + 1;
        } else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }
    return right;
}

/* squarefree table: sqfree[i] == 1 if i is squarefree, 0 otherwise */
static uint8_t *sqfree = NULL;
static size_t sqfree_cap = 0;

/* ensures squarefree info is available up to 'limit' (inclusive) */
static void ensure_squarefree(size_t limit) {
    if (limit + 1 <= sqfree_cap) return;             /* already computed */
    size_t new_cap = limit + 1;
    uint8_t *tmp = (uint8_t*)realloc(sqfree, new_cap); /* cast for C++ */
    if (!tmp) abort();
    sqfree = tmp;
    sqfree_cap = new_cap;
    /* mark all as squarefree initially */
    for (size_t i = 0; i < new_cap; ++i) sqfree[i] = 1;
    /* mark numbers divisible by a square of a prime */
    size_t sqrt_lim = int_sqrt(limit);
    for (size_t p = 2; p <= sqrt_lim; ++p) {
        bool is_prime = true;
        for (size_t q = 2; q*q <= p; ++q) {
            if (p % q == 0) { is_prime = false; break; }
        }
        if (!is_prime) continue;
        size_t sq = p * p;
        for (size_t m = sq; m <= limit; m += sq) {
            sqfree[m] = 0;
        }
    }
}

/* required function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    idx_t two_n = n + n;                     /* 2n, fits in idx_t for given constraints */
    ensure_squarefree((size_t)two_n);
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (sqfree[(size_t)i] == sqfree[(size_t)(two_n - i)]) cnt++;
    }
    return cnt;
}
