/*
 * A325528  Difference sequence of A325464.
 * https://oeis.org/A325528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A325464(n+1) - A325464(n), where A325464 is built
 *            greedily: a(1)=1, each next term is the least integer > previous
 *            term that is not 2*a(m) or 3*a(m)-2 for any m < n (search)
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9802 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static int A325464_forb_contains(const seq_t *arr, size_t len, seq_t x)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = arr[mid];
        if (midv < x)
            lo = mid + 1;
        else if (x < midv)
            hi = mid;
        else
            return 1;
    }
    return 0;
}

static void A325464_insert_sorted(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, *cap);
    }
    size_t lo = 0, hi = *len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((*arr)[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    for (size_t i = *len; i > lo; --i)
        (*arr)[i] = (*arr)[i - 1];
    (*arr)[lo] = val;
    (*len)++;
}

static seq_t *A325464_seq = NULL;
static size_t A325464_seq_len = 0;
static size_t A325464_seq_cap = 0;
static seq_t *A325464_forb = NULL;
static size_t A325464_forb_len = 0;
static size_t A325464_forb_cap = 0;

static seq_t A325464_a(idx_t n)
{
    if (A325464_seq_len == 0) {
        A325464_seq_cap = 16;
        A325464_seq = mem_new(seq_t, A325464_seq_cap);
        A325464_forb_cap = 32;
        A325464_forb = mem_new(seq_t, A325464_forb_cap);
        A325464_seq[0] = (seq_t)1;
        A325464_seq_len = 1;
        A325464_insert_sorted(&A325464_forb, &A325464_forb_len, &A325464_forb_cap, (seq_t)1);
        A325464_insert_sorted(&A325464_forb, &A325464_forb_len, &A325464_forb_cap, (seq_t)2);
    }
    while (A325464_seq_len < (size_t)n) {
        seq_t cand = A325464_seq[A325464_seq_len - 1] + (seq_t)1;
        while (A325464_forb_contains(A325464_forb, A325464_forb_len, cand))
            cand = cand + (seq_t)1;
        if (A325464_seq_len >= A325464_seq_cap) {
            A325464_seq_cap <<= 1;
            A325464_seq = mem_resize(seq_t, A325464_seq, A325464_seq_cap);
        }
        A325464_seq[A325464_seq_len++] = cand;
        A325464_insert_sorted(&A325464_forb, &A325464_forb_len, &A325464_forb_cap, cand * (seq_t)2);
        A325464_insert_sorted(&A325464_forb, &A325464_forb_len, &A325464_forb_cap, cand * (seq_t)3 - (seq_t)2);
    }
    return A325464_seq[n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t hi = A325464_a(n + 1);
    seq_t lo = A325464_a(n);
    return hi - lo;
}
