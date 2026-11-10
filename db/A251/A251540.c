/*
 * A251540  Odd numbers k such that A098548(k) is not a multiple of 3.
 * https://oeis.org/A251540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k such that A098548(k) mod 3 != 0; computed by
 *            iterating A098548 (a(1)=1,a(2)=2,a(3)=3, a(k)=least m>a(k-1)
 *            with gcd(m,a(k-2))>1, gcd(m,a(k-1))=1) and collecting qualifying
 *            odd k
 * Range:     u8 n <= 1, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 52 terms through n = 991 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static idx_t *A251540_found = 0;
static idx_t A251540_cnt = 0;
static idx_t A251540_cap = 0;
static idx_t A251540_k = 0;
static seq_t A251540_p2, A251540_p1;

static void A251540_push(idx_t k)
{
    if (A251540_cnt == A251540_cap) {
        A251540_cap = A251540_cap ? A251540_cap * 2 : 1024;
        A251540_found = (idx_t *)realloc(A251540_found, (size_t)A251540_cap * sizeof(idx_t));
    }
    A251540_found[A251540_cnt++] = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A251540_k == 0) {
        /* a(1)=1 qualifies (odd, not multiple of 3); a(3)=3 does not. */
        A251540_push(1);
        A251540_k = 3;
        A251540_p2 = (seq_t)2;
        A251540_p1 = (seq_t)3;
    }
    while (A251540_cnt < n) {
        seq_t cand = A251540_p1 + (seq_t)1;
        for (;;) {
            if (gcd(cand, A251540_p2) != (seq_t)1 &&
                gcd(cand, A251540_p1) == (seq_t)1)
                break;
            ++cand;
        }
        A251540_p2 = A251540_p1;
        A251540_p1 = cand;
        ++A251540_k;
        if ((A251540_k & 1) && (long long)(cand % (seq_t)3) != 0)
            A251540_push(A251540_k);
    }
    return (seq_t)A251540_found[n - 1];
}
