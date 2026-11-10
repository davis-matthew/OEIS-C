/*
 * A256283  Smallest m such that A257905(m) = n.
 * https://oeis.org/A256283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with A257905(m) = n; computed by simulating
 *            A257905 (Rule 3, a(1)=0, d(1)=0) until n first appears
 * Range:     u8 n <= 197, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 957 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static size_t *A256283_first = NULL;  /* first index m (1-based) where value v occurs, 0 = unseen */
static size_t  A256283_first_cap = 0;
static size_t *A256283_dpos = NULL;
static size_t  A256283_dpos_cap = 0;
static size_t *A256283_dneg = NULL;
static size_t  A256283_dneg_cap = 0;
static long long A256283_cur = 0;
static size_t A256283_count = 0;

static void A256283_grow(size_t **arr, size_t *cap, size_t idx)
{
    if (idx < *cap) return;
    size_t nc = *cap ? *cap * 2 : 1024;
    while (nc <= idx) nc *= 2;
    size_t *na = (size_t *)malloc(nc * sizeof(size_t));
    memset(na, 0, nc * sizeof(size_t));
    if (*arr) {
        memcpy(na, *arr, *cap * sizeof(size_t));
        free(*arr);
    }
    *arr = na;
    *cap = nc;
}

static void A256283_step(void)
{
    long long ak = A256283_cur;
    long long chosen = 0;
    bool found = false;
    long long h;
    for (h = 2 - ak; h < 0; h++) {
        size_t ni = (size_t)(-h);
        A256283_grow(&A256283_dneg, &A256283_dneg_cap, ni);
        if (A256283_dneg[ni]) continue;
        size_t av = (size_t)(ak + h);
        A256283_grow(&A256283_first, &A256283_first_cap, av);
        if (A256283_first[av]) continue;
        chosen = h;
        found = true;
        break;
    }
    if (!found) {
        for (h = 1; ; h++) {
            A256283_grow(&A256283_dpos, &A256283_dpos_cap, (size_t)h);
            if (A256283_dpos[(size_t)h]) continue;
            long long idx = ak - h;
            bool present = false;
            if (idx >= 0) {
                A256283_grow(&A256283_first, &A256283_first_cap, (size_t)idx);
                present = A256283_first[(size_t)idx] != 0;
            }
            if (!present) { chosen = h; break; }
        }
    }
    if (chosen >= 0) {
        A256283_grow(&A256283_dpos, &A256283_dpos_cap, (size_t)chosen);
        A256283_dpos[(size_t)chosen] = 1;
    } else {
        A256283_grow(&A256283_dneg, &A256283_dneg_cap, (size_t)(-chosen));
        A256283_dneg[(size_t)(-chosen)] = 1;
    }
    long long nx = ak + chosen;
    A256283_grow(&A256283_first, &A256283_first_cap, (size_t)nx);
    A256283_count++;
    if (!A256283_first[(size_t)nx]) A256283_first[(size_t)nx] = A256283_count;
    A256283_cur = nx;
}

seq_t a(idx_t n)
{
    if (A256283_count == 0) {
        A256283_grow(&A256283_first, &A256283_first_cap, 0);
        A256283_grow(&A256283_dpos, &A256283_dpos_cap, 0);
        A256283_grow(&A256283_dneg, &A256283_dneg_cap, 0);
        A256283_first[0] = 1;
        A256283_dpos[0] = 1;
        A256283_cur = 0;
        A256283_count = 1;
    }
    size_t v = (size_t)n;
    A256283_grow(&A256283_first, &A256283_first_cap, v);
    while (A256283_first[v] == 0) {
        A256283_step();
        A256283_grow(&A256283_first, &A256283_first_cap, v);
    }
    return (seq_t)A256283_first[v];
}
