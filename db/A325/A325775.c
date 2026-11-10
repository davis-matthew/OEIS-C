/*
 * A325775  Numbers that bring back the last digit of n on its first digit,
 *          using the "boomerang protocol" explained in A308306, with no
 *          duplicate term (except the -1 terms). See the Comments and Example
 *          sections for details.
 * https://oeis.org/A325775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   greedy search. a(n) = smallest unused m >= 0 whose boomerang
 *            displacement is the negative of n's (digit d gives +(d+1) if
 *            even, -(d+1) if odd); a(n) = -1 if n has displacement 0 (A308306
 *            term)
 * Range:     i8 n <= 70, i16 n <= 7776, i32 n <= 12000, i64 n <= 12000,
 *            i128 n <= 12000, big any n
 * Validated: standard, pass, 101 terms through n = 9612 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A325775_OFF 220
#define A325775_NB  441

static uint64_t *A325775_bk[A325775_NB];
static size_t A325775_len[A325775_NB];
static size_t A325775_cap[A325775_NB];
static size_t A325775_head[A325775_NB];
static uint64_t A325775_next_m = 0;

static uint64_t *A325775_res = NULL;
static size_t A325775_done = 0;
static size_t A325775_rcap = 0;

static int A325775_disp(uint64_t m)
{
    int s = 0;
    if (m == 0) return 1;
    while (m > 0) {
        int d = (int)(m % 10);
        m /= 10;
        if (d & 1) s -= d + 1; else s += d + 1;
    }
    return s;
}

static void A325775_gen(void)
{
    uint64_t m = A325775_next_m++;
    int d = A325775_disp(m);
    if (d == 0) return;
    int b = d + A325775_OFF;
    if (A325775_len[b] == A325775_cap[b]) {
        size_t nc = A325775_cap[b] ? A325775_cap[b] * 2 : 16;
        uint64_t *t = (uint64_t *)realloc(A325775_bk[b], nc * sizeof(uint64_t));
        if (!t) exit(1);
        A325775_bk[b] = t;
        A325775_cap[b] = nc;
    }
    A325775_bk[b][A325775_len[b]++] = m;
}

seq_t a(idx_t n)
{
    while (A325775_done < (size_t)n) {
        uint64_t k = (uint64_t)A325775_done + 1;
        int d = A325775_disp(k);
        uint64_t val;
        if (d == 0) {
            val = (uint64_t)(int64_t)-1;
        } else {
            int b = A325775_OFF - d;
            while (A325775_head[b] == A325775_len[b]) A325775_gen();
            val = A325775_bk[b][A325775_head[b]++];
        }
        if (A325775_done == A325775_rcap) {
            size_t nc = A325775_rcap ? A325775_rcap * 2 : 64;
            uint64_t *t = (uint64_t *)realloc(A325775_res, nc * sizeof(uint64_t));
            if (!t) exit(1);
            A325775_res = t;
            A325775_rcap = nc;
        }
        A325775_res[A325775_done++] = val;
    }
    uint64_t v = A325775_res[(size_t)n - 1];
    if (v == (uint64_t)(int64_t)-1) return (seq_t)-1;
    return (seq_t)v;
}
