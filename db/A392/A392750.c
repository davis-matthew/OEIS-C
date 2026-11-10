/*
 * A392750  Positive integers that appear in A381466.
 * https://oeis.org/A392750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   k is in the sequence iff k = A381466(i) for some i <= 6k^2-2k,
 *            where A381466(0)=4 and A381466(n) = A381466(n-1)+n if
 *            gcd(A381466(n-1),n)=1, else n/gcd(A381466(n-1),n). Computed by
 *            simulating A381466 and doubling the bound K on k until n members
 *            are found
 * Range:     u8 n <= 205, u16 n <= 1634, u32 n <= 1634, u64 n <= 1634,
 *            u128 n <= 1634, big any n
 * Validated: standard, pass, 93 terms through n = 1496 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A392750_list = NULL;
static idx_t A392750_cnt = 0;
static uint64_t A392750_K = 0;

static uint64_t A392750_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void A392750_build(uint64_t K)
{
    unsigned char *mark = (unsigned char *)calloc((size_t)K + 1, 1);
    uint64_t M = 6 * K * K - 2 * K;
    uint64_t prev = 4;
    if (prev <= K) mark[prev] = 1;
    for (uint64_t i = 1; i <= M; i++) {
        uint64_t g = A392750_gcd(prev, i);
        if (g == 1) prev = prev + i;
        else prev = i / g;
        if (prev <= K) mark[prev] = 1;
    }
    free(A392750_list);
    A392750_list = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)K + 1));
    A392750_cnt = 0;
    for (uint64_t k = 1; k <= K; k++)
        if (mark[k]) A392750_list[A392750_cnt++] = k;
    free(mark);
    A392750_K = K;
}

seq_t a(idx_t n)
{
    while (A392750_cnt < n) {
        uint64_t K = A392750_K ? A392750_K * 2 : 64;
        A392750_build(K);
    }
    return (seq_t)A392750_list[n - 1];
}
