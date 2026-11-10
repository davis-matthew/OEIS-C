/*
 * A307632  Index of first occurrence of n-th prime in A307720.
 * https://oeis.org/A307632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of first occurrence of prime(n) in A307720;
 *            computed by simulating A307720 (lexicographically earliest
 *            sequence with exactly k adjacent pairs of product k), with
 *            per-value "next candidate" pointers and compression of forced
 *            alternation runs v,y,v,y,
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 212, u64 n <= 1229,
 *            u128 n <= 1229, big any n
 * Validated: standard, pass, 53 terms through n = 1213 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A307632_cnt = NULL;   /* pairs used per product            */
static size_t A307632_cnt_cap = 0;
static uint32_t *A307632_ptr = NULL;   /* smallest y with v*y unsaturated   */
static size_t A307632_ptr_cap = 0;
static uint64_t *A307632_first = NULL; /* first position of each value      */
static size_t A307632_first_cap = 0;
static uint64_t A307632_pos = 0, A307632_prev = 0;
static uint64_t A307632_lfrom = 0, A307632_lto = 0;

static int A307632_isprime(uint64_t n)
{
    if (n < 2) return 0;
    for (uint64_t d = 2; d * d <= n; d++)
        if (n % d == 0) return 0;
    return 1;
}

static void A307632_grow32(uint32_t **arr, size_t *cap, size_t need)
{
    if (need < *cap) return;
    size_t nc = *cap ? *cap : 1024;
    while (nc <= need) nc <<= 1;
    *arr = (uint32_t *)realloc(*arr, nc * sizeof(uint32_t));
    memset(*arr + *cap, 0, (nc - *cap) * sizeof(uint32_t));
    *cap = nc;
}

static void A307632_grow64(uint64_t **arr, size_t *cap, size_t need)
{
    if (need < *cap) return;
    size_t nc = *cap ? *cap : 1024;
    while (nc <= need) nc <<= 1;
    *arr = (uint64_t *)realloc(*arr, nc * sizeof(uint64_t));
    memset(*arr + *cap, 0, (nc - *cap) * sizeof(uint64_t));
    *cap = nc;
}

static void A307632_step(void)
{
    uint64_t v = A307632_prev;
    A307632_grow32(&A307632_ptr, &A307632_ptr_cap, (size_t)v);
    uint64_t y = A307632_ptr[v] ? A307632_ptr[v] : 1;
    uint64_t P;
    for (;; y++) {
        P = v * y;
        A307632_grow32(&A307632_cnt, &A307632_cnt_cap, (size_t)P);
        if ((uint64_t)A307632_cnt[P] < P) break;
    }
    A307632_ptr[v] = (uint32_t)y;
    A307632_cnt[P]++;
    A307632_pos++;
    A307632_grow64(&A307632_first, &A307632_first_cap, (size_t)y);
    if (A307632_first[y] == 0) A307632_first[y] = A307632_pos;

    if (A307632_lfrom == y && A307632_lto == v) {
        /* forced alternation v,y,v,y,... until product P is saturated */
        uint64_t R = P - (uint64_t)A307632_cnt[P];
        A307632_cnt[P] = (uint32_t)P;
        A307632_pos += R;
        if (R & 1) {
            A307632_prev = v;
            A307632_lfrom = y;
            A307632_lto = v;
        } else {
            A307632_prev = y;
            A307632_lfrom = v;
            A307632_lto = y;
        }
    } else {
        A307632_lfrom = v;
        A307632_lto = y;
        A307632_prev = y;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A307632_pos == 0) {
        A307632_grow64(&A307632_first, &A307632_first_cap, 1);
        A307632_grow32(&A307632_cnt, &A307632_cnt_cap, 1);
        A307632_grow32(&A307632_ptr, &A307632_ptr_cap, 1);
        A307632_pos = 1;
        A307632_prev = 1;
        A307632_first[1] = 1;
        A307632_lfrom = 0;
        A307632_lto = 0;
    }
    uint64_t p = 1;
    idx_t c = 0;
    while (c < n) {
        p++;
        if (A307632_isprime(p)) c++;
    }
    for (;;) {
        if (p < A307632_first_cap && A307632_first[p] != 0)
            return (seq_t)A307632_first[p];
        A307632_step();
    }
}
