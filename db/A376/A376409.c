/*
 * A376409  a(n) = A048675(A376408(n)); Partial sums of A376408.
 * https://oeis.org/A376409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = Sum_{i=0..n-1} b(i) for n >= 1, where b =
 *            A376408: b(0) = 1, b(k+1) = b(k) * A019565(b(k)), and A019565(m)
 *            is the product of prime(i+1) over the set bits i of m. This
 *            follows from A048675(b(k+1)) = A048675(b(k)) + b(k)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 9 terms through n = 8 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A376409_pr = NULL;
static size_t A376409_cnt = 0;
static size_t A376409_cap = 0;

/* k-th prime, 1-based, from a growing cache */
static uint64_t A376409_prime(size_t k)
{
    if (A376409_cap == 0) {
        A376409_cap = 1024;
        A376409_pr = (uint64_t *)malloc(A376409_cap * sizeof(uint64_t));
        A376409_pr[0] = 2;
        A376409_cnt = 1;
    }
    while (A376409_cnt < k) {
        uint64_t c = A376409_pr[A376409_cnt - 1] + 1;
        if (c > 2 && (c & 1) == 0) c++;
        for (;; c += 2) {
            bool ok = true;
            for (size_t j = 0; j < A376409_cnt; j++) {
                uint64_t p = A376409_pr[j];
                if (p * p > c) break;
                if (c % p == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        if (A376409_cnt == A376409_cap) {
            A376409_cap *= 2;
            A376409_pr = (uint64_t *)realloc(A376409_pr, A376409_cap * sizeof(uint64_t));
        }
        A376409_pr[A376409_cnt++] = c;
    }
    return A376409_pr[k - 1];
}

/* A019565: product of prime(i+1) over set bits i of m */
static seq_t A376409_A019565(seq_t m)
{
    seq_t r = 1;
    size_t k = 1;
    while (m > 0) {
        if ((m & 1) != 0) r = r * (seq_t)A376409_prime(k);
        m >>= 1;
        k++;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t b = 1;
    seq_t sum = 0;
    for (idx_t i = 0; i < n; i++) {
        sum += b;
        if (i + 1 < n) {
            if (b == 0) break;
            b = b * A376409_A019565(b);
        }
    }
    return sum;
}
