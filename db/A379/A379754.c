/*
 * A379754  Records in A379752.
 * https://oeis.org/A379754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record value of A379752. A379752(k) depends only on
 *            the * exponents e_i of k: a(k) = (T - 2P - 2^w + Q + 2)/2 with T
 *            = prod(e_i+1), * P = prod(e_i), Q = prod(e_i-1), w = omega(k).
 *            Every record therefore occurs at * the smallest number with its
 *            exponent pattern, 2^e1*3^e2*... with * e1...
 * Range:     u8 n <= 48, u16 n <= 210, u32 n <= 226, u64 n <= 226,
 *            u128 n <= 226, big any n
 * Validated: standard, pass, 76 terms through n = 226 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

typedef unsigned __int128 A379754_u128;

typedef struct {
    A379754_u128 k;
    uint64_t f;
} A379754_item;

static const uint64_t A379754_primes[40] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173
};

static A379754_item *A379754_items = NULL;
static size_t A379754_cnt = 0, A379754_cap = 0;

static void A379754_push(A379754_u128 k, uint64_t f)
{
    if (A379754_cnt >= A379754_cap) {
        A379754_cap = A379754_cap ? A379754_cap * 2 : 1024;
        A379754_items = (A379754_item *)realloc(A379754_items,
                            A379754_cap * sizeof(A379754_item));
    }
    A379754_items[A379754_cnt].k = k;
    A379754_items[A379754_cnt].f = f;
    A379754_cnt++;
}

static void A379754_gen(int i, int maxe, A379754_u128 k, A379754_u128 B,
                        int64_t T, int64_t P, int64_t Q, int64_t W)
{
    if (i > 0) {
        int64_t ord = T - 2 * P - W + Q + 2;
        if (ord > 0) A379754_push(k, (uint64_t)(ord / 2));
    }
    if (i >= 40) return;
    uint64_t p = A379754_primes[i];
    A379754_u128 kk = k;
    for (int e = 1; e <= maxe; ++e) {
        if (kk > B / p) break;
        kk *= p;
        A379754_gen(i + 1, e, kk, B, T * (e + 1), P * e, Q * (e - 1), W * 2);
    }
}

static int A379754_cmp(const void *x, const void *y)
{
    const A379754_item *a1 = (const A379754_item *)x;
    const A379754_item *b1 = (const A379754_item *)y;
    if (a1->k < b1->k) return -1;
    if (a1->k > b1->k) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    static uint64_t *rec = NULL;
    static size_t rec_len = 0;
    static A379754_u128 bound = 0;

    if (n == 0) return 0;
    if (bound == 0) bound = (A379754_u128)1000000000000ULL;

    while (rec_len < (size_t)n) {
        A379754_cnt = 0;
        A379754_gen(0, 200, 1, bound, 1, 1, 1, 1);
        qsort(A379754_items, A379754_cnt, sizeof(A379754_item), A379754_cmp);
        free(rec);
        rec = (uint64_t *)malloc((A379754_cnt + 1) * sizeof(uint64_t));
        rec_len = 0;
        uint64_t best = 0;
        for (size_t i = 0; i < A379754_cnt; ++i) {
            if (A379754_items[i].f > best) {
                best = A379754_items[i].f;
                rec[rec_len++] = best;
            }
        }
        if (rec_len >= (size_t)n) break;
        if (bound > (((A379754_u128)1) << 110)) break;
        bound *= 10000;
    }
    if ((size_t)n > rec_len) return 0;
    return (seq_t)rec[n - 1];
}
