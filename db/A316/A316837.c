/*
 * A316837  Indices of 0's in A254990.
 * https://oeis.org/A316837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position (0-based) of the n-th 0 in A254990 = sum_{i<n-1}
 *            |image of A254990(i)| = 2*(n-1) - #{i<n-1 : A254990(i)=3}, where
 *            the morphism 0->01, 1->02, 2->03, 3->0 gives image lengths
 *            2,2,2,1
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 8875 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint8_t A254990_small[4][8] = {
    {0},
    {0, 1},
    {0, 1, 0, 2},
    {0, 1, 0, 2, 0, 1, 0, 3}
};

static uint64_t *A254990_len = NULL;
static size_t A254990_used = 0;
static size_t A254990_cap = 0;

static void A254990_ensure(uint64_t n)
{
    if (A254990_len == NULL) {
        A254990_cap = 8;
        A254990_len = (uint64_t *)malloc(A254990_cap * sizeof(uint64_t));
        A254990_len[0] = 1; A254990_len[1] = 2;
        A254990_len[2] = 4; A254990_len[3] = 8;
        A254990_used = 4;
    }
    while (A254990_len[A254990_used - 1] <= n) {
        if (A254990_cap < A254990_used + 2) {
            A254990_cap *= 2;
            A254990_len = (uint64_t *)realloc(A254990_len, A254990_cap * sizeof(uint64_t));
        }
        unsigned __int128 s =
            (unsigned __int128)A254990_len[A254990_used - 1] +
            A254990_len[A254990_used - 2] +
            A254990_len[A254990_used - 3] +
            A254990_len[A254990_used - 4];
        A254990_len[A254990_used] = (s > (unsigned __int128)UINT64_MAX) ? UINT64_MAX : (uint64_t)s;
        A254990_used++;
    }
}

static int A254990_at(uint64_t n)
{
    A254990_ensure(n);
    size_t k = 0;
    while (k + 1 < A254990_used && A254990_len[k] <= n) k++;
    uint64_t pos = n;
    while (k > 3) {
        uint64_t l1 = A254990_len[k - 1];
        uint64_t l2 = A254990_len[k - 2];
        uint64_t l3 = A254990_len[k - 3];
        if (pos < l1) {
            k--;
        } else if (pos < l1 + l2) {
            pos -= l1; k -= 2;
        } else if (pos < l1 + l2 + l3) {
            pos -= l1 + l2; k -= 3;
        } else {
            pos -= l1 + l2 + l3; k -= 4;
        }
    }
    return (int)A254990_small[k][(size_t)pos];
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n - 1;
    uint64_t threes = 0;
    for (uint64_t i = 0; i < k; i++) {
        if (A254990_at(i) == 3) threes++;
    }
    return (seq_t)(2 * k - threes);
}
