/*
 * A251537  Odd numbers n such that A098548(n+2) > A098548(n) + 6.
 * https://oeis.org/A251537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k with A098548(k+2) > A098548(k) + 6;
 *            A098548(1..3)=1,2,3, then smallest m > a(n-1) with
 *            gcd(m,a(n-2))>1 and gcd(m,a(n-1))=1; computed by direct
 *            generation and scan
 * Range:     u8 n <= 8, u16 n <= 3047, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 64 terms through n = 9731 (2026-11-07)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *A251537_seqv = 0;   /* A098548 terms, 1-indexed */
static idx_t A251537_cnt = 0;     /* number of terms stored */
static idx_t A251537_cap = 0;
static idx_t *A251537_found = 0;
static idx_t A251537_fcnt = 0;
static idx_t A251537_fcap = 0;
static idx_t A251537_next = 1;    /* next odd index to test */

static void A251537_ensure(idx_t m)
{
    if (m <= A251537_cnt) return;
    if (m + 1 > A251537_cap) {
        idx_t nc = A251537_cap ? A251537_cap * 2 : 1024;
        while (nc < m + 1) nc *= 2;
        if (A251537_seqv == 0) {
            A251537_seqv = mem_new(seq_t, nc);
        } else {
            A251537_seqv = mem_resize(seq_t, A251537_seqv, nc);
        }
        A251537_cap = nc;
    }
    if (A251537_cnt == 0) {
        A251537_seqv[1] = (seq_t)1;
        A251537_seqv[2] = (seq_t)2;
        A251537_seqv[3] = (seq_t)3;
        A251537_cnt = 3;
    }
    while (A251537_cnt < m) {
        seq_t p2 = A251537_seqv[A251537_cnt - 1];
        seq_t p1 = A251537_seqv[A251537_cnt];
        seq_t c = p1 + (seq_t)1;
        for (;;) {
            if (gcd(c, p2) != (seq_t)1 && gcd(c, p1) == (seq_t)1) break;
            ++c;
        }
        A251537_cnt++;
        A251537_seqv[A251537_cnt] = c;
    }
}

seq_t a(idx_t n)
{
    while (A251537_fcnt < n) {
        idx_t k = A251537_next;
        A251537_ensure(k + 2);
        if (A251537_seqv[k + 2] > A251537_seqv[k] + (seq_t)6) {
            if (A251537_fcnt + 1 > A251537_fcap) {
                idx_t nc = A251537_fcap ? A251537_fcap * 2 : 1024;
                if (A251537_found == 0) {
                    A251537_found = mem_new(idx_t, nc);
                } else {
                    A251537_found = mem_resize(idx_t, A251537_found, nc);
                }
                A251537_fcap = nc;
            }
            A251537_found[A251537_fcnt++] = k;
        }
        A251537_next = k + 2;
    }
    return (seq_t)A251537_found[n - 1];
}
