/*
 * A257271  Inverse of the permutation A257470.
 * https://oeis.org/A257271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   inverse permutation of A257470; a(n) = k such that A257470(k) =
 *            n. Computed by generating A257470 (a(0)=0, a(k+1)=least unused m
 *            with max(a(k),m)>=2*min(a(k),m)) until n appears
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *inv_arr = NULL;   /* inv_arr[v] = index+1 where value v occurs, 0 if not yet */
static size_t inv_cap = 0;
static uint64_t gen_len = 0;       /* number of terms generated */
static uint64_t gen_prev = 0;
static uint64_t gen_low = 0;       /* smallest unused value */

static void A257271_ensure(size_t need)
{
    if (inv_cap > need) return;
    size_t nc = inv_cap ? inv_cap * 2 : 1024;
    while (nc <= need) nc *= 2;
    uint64_t *t = (uint64_t *)realloc(inv_arr, nc * sizeof(uint64_t));
    if (!t) exit(EXIT_FAILURE);
    for (size_t i = inv_cap; i < nc; i++) t[i] = 0;
    inv_arr = t;
    inv_cap = nc;
}

seq_t a(idx_t n)
{
    if (gen_len == 0) {
        A257271_ensure(1024);
        inv_arr[0] = 1;
        gen_len = 1;
        gen_prev = 0;
        gen_low = 1;
    }
    for (;;) {
        A257271_ensure((size_t)n);
        if (inv_arr[(size_t)n]) break;
        uint64_t half = gen_prev / 2;
        uint64_t cand;
        while (1) {
            A257271_ensure((size_t)gen_low);
            if (!inv_arr[gen_low]) break;
            gen_low++;
        }
        if (gen_low <= half) {
            cand = gen_low;
        } else {
            cand = gen_prev * 2;
            while (1) {
                A257271_ensure((size_t)cand);
                if (!inv_arr[cand]) break;
                cand++;
            }
        }
        A257271_ensure((size_t)cand);
        inv_arr[cand] = gen_len + 1;
        gen_len++;
        gen_prev = cand;
    }
    return (seq_t)(inv_arr[(size_t)n] - 1);
}
