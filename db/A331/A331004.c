/*
 * A331004  a(n) is the real part of f(n) (as defined in A330780).
 * https://oeis.org/A331004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Re(f(n)), f(n) = Sum_{k=1..n} b(k)*m(k), where b is
 *            A330780 (greedy search, injective partial sums) and m(k) is the
 *            knight move (2+i, 1+2i, -1+2i, -2+i, -2-i, -1-2i, 1-2i, 2-i)
 *            repeated with period 8; a(0) = 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9605 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const int64_t A331004_mre[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
static const int64_t A331004_mim[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

static size_t A331004_done = 0;
static int64_t *A331004_sr = NULL;
static int64_t *A331004_si = NULL;
static size_t A331004_scap = 0;
static int *A331004_counts = NULL;
static size_t A331004_ccap = 0;
static size_t *A331004_tab = NULL;
static size_t A331004_tcap = 0;
static size_t A331004_tcnt = 0;

static size_t A331004_hash(int64_t r, int64_t i)
{
    uint64_t h = (uint64_t)r * 0x9E3779B97F4A7C15ULL ^ ((uint64_t)i + 0x7F4A7C15ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    h *= 0xBF58476D1CE4E5B9ULL;
    h ^= h >> 32;
    return (size_t)h;
}

static bool A331004_find(int64_t r, int64_t i)
{
    size_t h = A331004_hash(r, i) & (A331004_tcap - 1);
    while (A331004_tab[h]) {
        size_t k = A331004_tab[h] - 1;
        if (A331004_sr[k] == r && A331004_si[k] == i) return true;
        h = (h + 1) & (A331004_tcap - 1);
    }
    return false;
}

static void A331004_insert_idx(size_t k)
{
    size_t h = A331004_hash(A331004_sr[k], A331004_si[k]) & (A331004_tcap - 1);
    while (A331004_tab[h]) h = (h + 1) & (A331004_tcap - 1);
    A331004_tab[h] = k + 1;
    A331004_tcnt++;
}

static void A331004_add(size_t k)
{
    if ((A331004_tcnt + 1) * 2 > A331004_tcap) {
        size_t nc = A331004_tcap ? A331004_tcap * 2 : 1024;
        free(A331004_tab);
        A331004_tab = (size_t *)calloc(nc, sizeof(size_t));
        A331004_tcap = nc;
        A331004_tcnt = 0;
        for (size_t j = 0; j < k; j++) A331004_insert_idx(j);
    }
    A331004_insert_idx(k);
}

static void A331004_init(void)
{
    A331004_scap = 1024;
    A331004_sr = (int64_t *)malloc(A331004_scap * sizeof(int64_t));
    A331004_si = (int64_t *)malloc(A331004_scap * sizeof(int64_t));
    A331004_ccap = 256;
    A331004_counts = (int *)calloc(A331004_ccap, sizeof(int));
    A331004_sr[0] = 0;
    A331004_si[0] = 0;
    A331004_add(0);
}

seq_t a(idx_t n)
{
    if (!A331004_sr) A331004_init();
    while (A331004_done < (size_t)n) {
        size_t pos = A331004_done % 8;
        int64_t cr = A331004_sr[A331004_done];
        int64_t ci = A331004_si[A331004_done];
        for (int64_t val = 1;; ++val) {
            if ((size_t)val >= A331004_ccap) {
                size_t nc = A331004_ccap * 2;
                while (nc <= (size_t)val) nc *= 2;
                A331004_counts = (int *)realloc(A331004_counts, nc * sizeof(int));
                memset(A331004_counts + A331004_ccap, 0, (nc - A331004_ccap) * sizeof(int));
                A331004_ccap = nc;
            }
            if ((int64_t)A331004_counts[val] >= val * val) continue;
            int64_t nr = cr + val * A331004_mre[pos];
            int64_t ni = ci + val * A331004_mim[pos];
            if (A331004_find(nr, ni)) continue;
            if (A331004_done + 2 > A331004_scap) {
                A331004_scap *= 2;
                A331004_sr = (int64_t *)realloc(A331004_sr, A331004_scap * sizeof(int64_t));
                A331004_si = (int64_t *)realloc(A331004_si, A331004_scap * sizeof(int64_t));
            }
            A331004_counts[val]++;
            A331004_sr[A331004_done + 1] = nr;
            A331004_si[A331004_done + 1] = ni;
            A331004_add(A331004_done + 1);
            A331004_done++;
            break;
        }
    }
    return (seq_t)A331004_sr[n];
}
