/*
 * A294576  Odd numbers n such that 2 * A243823(n) > n.
 * https://oeis.org/A294576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd m with 2*A243823(m) > m, where A243823(m) =
 *            m-1-phi(m) - #{k in 2..m-1 : every prime factor of k divides m};
 *            found by a memoized scan over odd m
 * Range:     u8 none, u16 n <= 689, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 8458 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t A294576_ps[24];
static int A294576_np;
static uint64_t A294576_lim;
static uint64_t A294576_c;

static void A294576_dfs(int start, uint64_t cur)
{
    for (int j = start; j < A294576_np; ++j) {
        uint64_t v = cur * A294576_ps[j];
        if (v < A294576_lim) {
            ++A294576_c;
            A294576_dfs(j, v);
        }
    }
}

static bool A294576_test(uint64_t m)
{
    if (m < 3) return false;
    uint64_t t = m, phi = m;
    A294576_np = 0;
    for (uint64_t p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            A294576_ps[A294576_np++] = p;
            phi = phi / p * (p - 1);
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) {
        A294576_ps[A294576_np++] = t;
        phi = phi / t * (t - 1);
    }
    A294576_lim = m;
    A294576_c = 0;
    A294576_dfs(0, 1);
    uint64_t a243823 = (m - 1 - phi) - A294576_c;
    return 2 * a243823 > m;
}

static uint64_t *A294576_terms = NULL;
static size_t A294576_cnt = 0, A294576_cap = 0;
static uint64_t A294576_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A294576_cnt < (size_t)n) {
        uint64_t m = A294576_next;
        A294576_next += 2;
        if (A294576_test(m)) {
            if (A294576_cnt == A294576_cap) {
                A294576_cap = A294576_cap ? A294576_cap * 2 : 256;
                A294576_terms = (uint64_t *)realloc(A294576_terms, A294576_cap * sizeof(uint64_t));
            }
            A294576_terms[A294576_cnt++] = m;
        }
    }
    return (seq_t)A294576_terms[n - 1];
}
