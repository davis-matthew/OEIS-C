/*
 * A269739  Index in A092243 where n first appears.
 * https://oeis.org/A269739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 with A092243(k) = n; found by a search:
 *            segmented prime sieve, running tug-of-war score over prime gaps,
 *            recording the index each time the score reaches a new maximum
 * Range:     u8 n <= 17, u16 n <= 93, u32 n <= 3608, u64 n <= 3608,
 *            u128 n <= 3608, big any n
 * Validated: standard, pass, 72 terms through n = 2668 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define A269739_SEG ((size_t)1 << 22)

static uint64_t *A269739_rec = NULL;
static size_t A269739_len = 0, A269739_cap = 0;
static uint32_t *A269739_base = NULL;
static size_t A269739_nbase = 0;
static unsigned char *A269739_flag = NULL;
static uint64_t A269739_lo = 0;
static bool A269739_init = false;
static uint64_t A269739_lastp = 0;
static int64_t A269739_lastgap = 0;
static int64_t A269739_score = 0;
static uint64_t A269739_gidx = 0;
static bool A269739_havep = false, A269739_havegap = false;

static void A269739_push(uint64_t v)
{
    if (A269739_len == A269739_cap) {
        A269739_cap = A269739_cap ? A269739_cap * 2 : 1024;
        A269739_rec = (uint64_t *)realloc(A269739_rec, A269739_cap * sizeof(uint64_t));
        if (!A269739_rec) abort();
    }
    A269739_rec[A269739_len++] = v;
}

static void A269739_prime(uint64_t p)
{
    if (!A269739_havep) {
        A269739_havep = true;
        A269739_lastp = p;
        return;
    }
    int64_t gap = (int64_t)(p - A269739_lastp);
    A269739_lastp = p;
    A269739_gidx++;
    if (!A269739_havegap) {
        A269739_havegap = true;
        A269739_score = 0;
        A269739_push(A269739_gidx); /* score 0 first at index 1 */
    } else {
        if (gap > A269739_lastgap) A269739_score++;
        else if (gap < A269739_lastgap) A269739_score--;
        if (A269739_score >= 0 && (uint64_t)A269739_score == (uint64_t)A269739_len)
            A269739_push(A269739_gidx);
    }
    A269739_lastgap = gap;
}

seq_t a(idx_t n)
{
    if (!A269739_init) {
        A269739_init = true;
        A269739_flag = (unsigned char *)malloc(A269739_SEG);
        A269739_base = (uint32_t *)malloc(sizeof(uint32_t) * 400000);
        if (!A269739_flag || !A269739_base) abort();
    }
    while (A269739_len <= (size_t)n) {
        uint64_t lo = A269739_lo, hi = lo + (uint64_t)A269739_SEG;
        for (size_t i = 0; i < A269739_SEG; i++) A269739_flag[i] = 1;
        if (lo == 0) {
            A269739_flag[0] = 0;
            A269739_flag[1] = 0;
            for (uint64_t i = 2; i * i < hi; i++)
                if (A269739_flag[i])
                    for (uint64_t j = i * i; j < hi; j += i) A269739_flag[j] = 0;
            for (uint64_t i = 2; i < hi; i++)
                if (A269739_flag[i]) A269739_base[A269739_nbase++] = (uint32_t)i;
        } else {
            for (size_t b = 0; b < A269739_nbase; b++) {
                uint64_t p = A269739_base[b];
                if (p * p >= hi) break;
                uint64_t s = (lo + p - 1) / p * p;
                if (s < p * p) s = p * p;
                for (uint64_t j = s; j < hi; j += p) A269739_flag[j - lo] = 0;
            }
        }
        for (size_t i = 0; i < A269739_SEG; i++)
            if (A269739_flag[i]) A269739_prime(lo + i);
        A269739_lo = hi;
    }
    return (seq_t)A269739_rec[(size_t)n];
}
