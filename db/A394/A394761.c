/*
 * A394761  a(n) = the length of the n-th run of 1's in A392200.
 * https://oeis.org/A394761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A391446(n) - A391446(n-1) - 1, i.e. the number of
 *            addition steps between consecutive division steps of A381466
 *            (A381466(0)=4; step k: if g=gcd(prev,k)=1 add k, else k/g).
 *            Computed by direct simulation
 * Range:     u8 n <= 28, u16 n <= 6943, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 103 terms through n = 8274 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static unsigned char *A394761_runs = NULL;
static size_t A394761_cnt = 0;
static size_t A394761_cap = 0;
static int A394761_init = 0;
static seq_t A394761_cur;
static idx_t A394761_step = 0;
static unsigned A394761_run = 0;

seq_t a(idx_t n)
{
    if (!A394761_init) {
        A394761_cur = (seq_t)4;
        A394761_init = 1;
    }
    while ((idx_t)A394761_cnt <= n) {
        A394761_step++;
        seq_t k = (seq_t)A394761_step;
        seq_t g = gcd(A394761_cur, k);
        if (g == (seq_t)1) {
            A394761_cur = A394761_cur + k;
            A394761_run++;
        } else {
            A394761_cur = k / g;
            if (A394761_cnt >= A394761_cap) {
                size_t nc = A394761_cap ? A394761_cap * 2 : 1024;
                unsigned char *t = (unsigned char *)realloc(A394761_runs, nc);
                if (!t) abort();
                A394761_runs = t;
                A394761_cap = nc;
            }
            A394761_runs[A394761_cnt++] = (unsigned char)A394761_run;
            A394761_run = 0;
        }
    }
    return (seq_t)A394761_runs[n];
}
