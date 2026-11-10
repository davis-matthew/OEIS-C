/*
 * A390509  Indices k such that (number of 0's) = (number of 1's) in the
 *          NA-run 01-word indexed by a(k) in A390508(k).
 * https://oeis.org/A390509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that the 01-word with A076478 index
 *            A390508(k) has equal numbers of 0's and 1's (found by search)
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A390509_u128;

static int A390509_msb(uint64_t x)
{
    int r = 0;
    while (x >>= 1) ++r;
    return r;
}

/* A390508(k) as a 128-bit value */
static A390509_u128 A390509_na_index(uint64_t k)
{
    uint64_t cumulative = 0;
    int L = 0;
    while (k > cumulative) {
        ++L;
        cumulative += (uint64_t)1 << L;
    }
    uint64_t prev = cumulative - ((uint64_t)1 << L);
    uint64_t offset = k - prev - 1;

    A390509_u128 r = 0;
    int lenNA = 0;
    int cur = (int)((offset >> (L - 1)) & 1);
    uint64_t run = 1;
    for (int pos = L - 2; pos >= -1; --pos) {
        int bit = (pos >= 0) ? (int)((offset >> pos) & 1) : -1;
        if (pos >= 0 && bit == cur) {
            ++run;
        } else {
            r = (r << 1) | (A390509_u128)cur;
            ++lenNA;
            int nb = A390509_msb(run) + 1;
            for (int b = nb - 1; b >= 0; --b) {
                r = (r << 1) | (A390509_u128)((run >> b) & 1);
                ++lenNA;
            }
            cur = bit;
            run = 1;
        }
    }
    return (((A390509_u128)1 << lenNA) - 1) + r;
}

static int A390509_balanced(uint64_t k)
{
    A390509_u128 m = A390509_na_index(k) + 1; /* word of length L2 has 2^L2 <= m+1 < 2^(L2+1) */
    int L2 = 0;
    A390509_u128 t = m;
    while (t >>= 1) ++L2;
    A390509_u128 v = m - ((A390509_u128)1 << L2);
    int ones = 0;
    while (v) {
        ones += (int)(v & 1);
        v >>= 1;
    }
    return 2 * ones == L2;
}

#define A390509_CACHE 4096
static uint64_t A390509_found[A390509_CACHE];
static uint64_t A390509_count = 0;
static uint64_t A390509_last = 0;

seq_t a(idx_t n)
{
    while (A390509_count < n && A390509_count < A390509_CACHE) {
        ++A390509_last;
        if (A390509_balanced(A390509_last))
            A390509_found[A390509_count++] = A390509_last;
    }
    if (n <= A390509_CACHE)
        return (seq_t)A390509_found[n - 1];
    uint64_t c = A390509_count, k = A390509_last;
    while (c < n) {
        ++k;
        if (A390509_balanced(k)) ++c;
    }
    return (seq_t)k;
}
