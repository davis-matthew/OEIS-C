/*
 * A391446  Numbers k such that gcd(A381466(k-1),k) > 1.
 * https://oeis.org/A391446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with gcd(A381466(k-1),k) > 1, where
 *            A381466(0)=4, A381466(k)=A381466(k-1)+k if gcd(A381466(k-1),k)=1
 *            else k/gcd(A381466(k-1),k); computed by iterating the recurrence
 * Range:     u8 n <= 29, u16 n <= 7247, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 75 terms through n = 9420 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static idx_t *A391446_found = NULL;
static size_t A391446_cnt = 0;
static size_t A391446_cap = 0;
static seq_t A391446_cur = 4;
static idx_t A391446_k = 0;
static int A391446_init = 0;

seq_t a(idx_t n)
{
    if (!A391446_init) {
        A391446_cur = (seq_t)4;
        A391446_k = 0;
        A391446_init = 1;
    }
    while (A391446_cnt < (size_t)n) {
        idx_t k = A391446_k + 1;
        seq_t kk = (seq_t)k;
        seq_t g = gcd(A391446_cur, kk);
        if (g == (seq_t)1) {
            A391446_cur = A391446_cur + kk;
        } else {
            A391446_cur = kk / g;
            if (A391446_cnt >= A391446_cap) {
                size_t nc = A391446_cap ? A391446_cap * 2 : 64;
                idx_t *t = mem_resize(idx_t, A391446_found, nc);
                if (!t) abort();
                A391446_found = t;
                A391446_cap = nc;
            }
            A391446_found[A391446_cnt++] = k;
        }
        A391446_k = k;
    }
    return (seq_t)A391446_found[n - 1];
}
