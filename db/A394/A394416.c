/*
 * A394416  a(1) = 1, a(2) = 2; for n > 2, a(n) is the greatest unused number
 *          < a(n-1) such that it shares a factor with a(n-1) while
 *          rad(a(n)*a(n-1)) has not previously occurred as the rad value of
 *          the product of adjacent terms, where rad(k) = A007947(k). If no
 *          such number exists then a(n) is the smallest n...
 * https://oeis.org/A394416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 174, u16 n <= 8090, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 549, big any n
 * Helpers:   gcd, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry A394416:
   a(1)=1, a(2)=2; for n>2, a(n) is the greatest unused number < a(n‑1)
   that shares a factor with a(n‑1) and for which rad(a(n)*a(n‑1))
   has not appeared before.  If no such number exists, use the smallest
   number > a(n‑1) satisfying the same criterion.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/rad.h"
#include "../utils/gcd.h"

static size_t seq_cap   = 0;          /* capacity of seq_arr (index 0 unused)      */
static seq_t *seq_arr   = NULL;       /* stored sequence values                     */

static size_t used_cap  = 0;          /* capacity of used_nums                     */
static bool   *used_nums = NULL;      /* bitmap of already used numbers           */

static size_t rad_cap   = 0;          /* capacity of rad_vals                      */
static size_t rad_cnt   = 0;          /* number of stored rad values               */
static idx_t  *rad_vals = NULL;      /* list of rad values seen so far           */

/*--- helpers to grow dynamic arrays -----------------------------------*/
static void ensure_seq_cap(idx_t n)
{
    if ((size_t)n + 1 > seq_cap) {
        size_t newcap = seq_cap ? seq_cap * 2 : 16;
        while ((size_t)n + 1 > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq_arr = tmp;
        seq_cap = newcap;
    }
}

static void ensure_used_cap(idx_t v)
{
    if ((size_t)v + 1 > used_cap) {
        size_t newcap = used_cap ? used_cap * 2 : 16;
        while ((size_t)v + 1 > newcap) newcap <<= 1;
        bool *tmp = (bool *)realloc(used_nums, newcap * sizeof(bool));
        if (!tmp) exit(EXIT_FAILURE);
        memset(tmp + used_cap, 0, (newcap - used_cap) * sizeof(bool));
        used_nums = tmp;
        used_cap  = newcap;
    }
}

static void ensure_rad_cap(void)
{
    if (rad_cnt + 1 > rad_cap) {
        size_t newcap = rad_cap ? rad_cap * 2 : 16;
        while (rad_cnt + 1 > newcap) newcap <<= 1;
        idx_t *tmp = (idx_t *)realloc(rad_vals, newcap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);
        rad_vals = tmp;
        rad_cap  = newcap;
    }
}

/*--- check whether a rad value is already used ------------------------*/
static bool rad_has_value(idx_t v)
{
    for (size_t i = 0; i < rad_cnt; ++i)
        if (rad_vals[i] == v) return true;
    return false;
}

/*--- record a new rad value ------------------------------------------*/
static void rad_mark(idx_t v)
{
    ensure_rad_cap();
    rad_vals[rad_cnt++] = v;
}

/*--- compute rad of product a*b using the multiplicative property ----*/
static idx_t rad_of_product(seq_t a, seq_t b)
{
    /* rad(a*b) = rad(a)*rad(b)/rad(gcd(a,b)) */
    seq_t g  = gcd(a, b);                /* seq_t result */
    idx_t rad_a = (idx_t)rad((idx_t)a);
    idx_t rad_b = (idx_t)rad((idx_t)b);
    idx_t rad_g = (idx_t)rad((idx_t)g);
    __uint128_t mult = ( __uint128_t)rad_a * rad_b;
    return (idx_t)(mult / rad_g);
}

/*--- choose next term given the current previous term               */
static seq_t pick_next(seq_t current)
{
    idx_t curIdx = (idx_t)current;

    /* search downward for the greatest suitable candidate */
    for (idx_t x = curIdx - 1; x > 0; --x) {
        if (!used_nums[x]) {
            seq_t g = gcd(current, (seq_t)x);
            if (g > 1) {
                idx_t r = rad_of_product(current, (seq_t)x);
                if (!rad_has_value(r)) return (seq_t)x;
            }
        }
    }

    /* no downward candidate: search upward */
    for (idx_t x = curIdx + 1;; ++x) {
        ensure_used_cap(x);
        if (!used_nums[x]) {
            seq_t g = gcd(current, (seq_t)x);
            if (g > 1) {
                idx_t r = rad_of_product(current, (seq_t)x);
                if (!rad_has_value(r)) return (seq_t)x;
            }
        }
    }
}

/*--- main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for 0 */

    /* initialise on first call */
    if (seq_cap == 0) {
        ensure_seq_cap(2);
        seq_arr[1] = (seq_t)1;
        seq_arr[2] = (seq_t)2;
        ensure_used_cap(2);
        used_nums[1] = true;
        used_nums[2] = true;
        rad_mark(rad_of_product((seq_t)1, (seq_t)2)); /* rad(1*2)=2 */
    }

    /* compute sequentially up to requested n */
    if ((size_t)n >= seq_cap) ensure_seq_cap(n);
    for (idx_t i = 3; i <= n; ++i) {
        if (seq_arr[i] != 0) continue;   /* already computed */
        seq_t prev = seq_arr[i - 1];
        seq_t nxt  = pick_next(prev);
        seq_arr[i] = nxt;

        /* mark usage */
        idx_t nxtIdx = (idx_t)nxt;
        ensure_used_cap(nxtIdx);
        used_nums[nxtIdx] = true;

        /* record rad value of adjacent product */
        rad_mark(rad_of_product(prev, nxt));
    }

    return seq_arr[n];
}
