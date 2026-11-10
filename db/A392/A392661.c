/*
 * A392661  Upper Wythoff partition sequence of the triangular numbers
 *          (A003086); see A392660.
 * https://oeis.org/A392661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   S = triangular numbers T(k) = k*(k+1)/2. Built greedily: L(n) =
 *            smallest member of S not used yet; a(n) = U(n) = smallest member
 *            of S with U(n) >= L(n) + T(n). A392660 is L. The code runs this
 *            construction up to n
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *A392661_uidx = NULL;   /* uidx[k] = index j with U(k) = T(j) */
static idx_t A392661_cnt = 0;           /* number of terms computed */
static unsigned char *A392661_used = NULL;
static size_t A392661_cap = 0;
static uint64_t A392661_lp = 1;         /* mex pointer over triangular indices */
static uint64_t A392661_up = 0;         /* last upper index */
static size_t A392661_ucap = 0;

static unsigned __int128 A392661_T(uint64_t k)
{
    return ((unsigned __int128)k * (unsigned __int128)(k + 1)) / 2;
}

static void A392661_ensure(size_t need)
{
    if (need < A392661_cap) return;
    size_t nc = A392661_cap ? A392661_cap : 1024;
    while (nc <= need) nc *= 2;
    A392661_used = (unsigned char *)realloc(A392661_used, nc);
    memset(A392661_used + A392661_cap, 0, nc - A392661_cap);
    A392661_cap = nc;
}

static void A392661_extend(idx_t n)
{
    if (n + 1 > A392661_ucap) {
        size_t nc = A392661_ucap ? A392661_ucap : 256;
        while (nc < n + 1) nc *= 2;
        A392661_uidx = (uint64_t *)realloc(A392661_uidx, nc * sizeof(uint64_t));
        A392661_ucap = nc;
    }
    while (A392661_cnt < n) {
        idx_t k = A392661_cnt + 1;
        A392661_ensure((size_t)A392661_lp + 1);
        while (A392661_used[A392661_lp]) {
            A392661_lp++;
            A392661_ensure((size_t)A392661_lp + 1);
        }
        uint64_t L = A392661_lp;
        A392661_used[L] = 1;
        unsigned __int128 target = A392661_T(L) + A392661_T(k);
        uint64_t j = A392661_up + 1;
        if (j < L + 1) j = L + 1;
        while (A392661_T(j) < target) j++;
        A392661_ensure((size_t)j + 1);
        A392661_used[j] = 1;
        A392661_up = j;
        A392661_uidx[k] = j;
        A392661_cnt = k;
    }
}

seq_t a(idx_t n)
{
    A392661_extend(n);
    uint64_t j = A392661_uidx[n];
    seq_t r;
    if ((j & 1) == 0) {
        r = (seq_t)(j / 2);
        r *= (seq_t)(j + 1);
    } else {
        r = (seq_t)j;
        r *= (seq_t)((j + 1) / 2);
    }
    return r;
}
