/*
 * A297827  Difference sequence of A297826.
 * https://oeis.org/A297827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A297826(n) - A297826(n-1), where A297826: c(0)=1, c(1)=2,
 *            c(n) = 2*b(n-1) - b(n-2) + n, b = increasing positive integers
 *            not in c
 * Range:     u8 n <= 367, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9697 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A297826_ac = NULL;
static seq_t *A297826_bc = NULL;
static size_t A297826_an = 0, A297826_acap = 0;
static size_t A297826_bn = 0, A297826_bcap = 0;
static seq_t A297826_next = 1;

static bool A297826_contains(seq_t val)
{
    size_t lo = 0, hi = A297826_an;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (A297826_ac[mid] < val) lo = mid + 1;
        else if (A297826_ac[mid] > val) hi = mid;
        else return true;
    }
    return false;
}

static seq_t A297826_next_b(void)
{
    seq_t val = A297826_next;
    while (A297826_contains(val)) ++val;
    A297826_next = val + 1;
    return val;
}

static void A297826_push_a(seq_t v)
{
    if (A297826_ac == NULL) {
        A297826_acap = 16;
        A297826_ac = mem_new(seq_t, A297826_acap);
    } else if (A297826_an >= A297826_acap) {
        A297826_acap *= 2;
        A297826_ac = mem_resize(seq_t, A297826_ac, A297826_acap);
    }
    A297826_ac[A297826_an++] = v;
}

static void A297826_push_b(seq_t v)
{
    if (A297826_bc == NULL) {
        A297826_bcap = 16;
        A297826_bc = mem_new(seq_t, A297826_bcap);
    } else if (A297826_bn >= A297826_bcap) {
        A297826_bcap *= 2;
        A297826_bc = mem_resize(seq_t, A297826_bc, A297826_bcap);
    }
    A297826_bc[A297826_bn++] = v;
}

static void A297826_ensure(size_t n)
{
    if (A297826_an == 0) {
        A297826_push_a((seq_t)1);
        A297826_push_a((seq_t)2);
        A297826_push_b(A297826_next_b());
        A297826_push_b(A297826_next_b());
    }
    while (A297826_an <= n) {
        size_t cur = A297826_an;
        while (A297826_bn <= cur - 1)
            A297826_push_b(A297826_next_b());
        seq_t v = (seq_t)2 * A297826_bc[cur - 1] - A297826_bc[cur - 2] + (seq_t)cur;
        A297826_push_a(v);
    }
}

seq_t a(idx_t n)
{
    size_t k = (size_t)n;
    A297826_ensure(k);
    return A297826_ac[k] - A297826_ac[k - 1];
}
