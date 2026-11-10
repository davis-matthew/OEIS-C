/*
 * A396082  Primes p such that A381466(2p) > 2p.
 * https://oeis.org/A396082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p (in increasing order) with A381466(2p) > 2p,
 *            where A381466(0)=4, A381466(m)=A381466(m-1)+m if
 *            gcd(A381466(m-1),m)=1 else m/gcd. Computed by simulating A381466
 *            incrementally
 * Range:     u8 n <= 9, u16 n <= 660, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9935 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *A396082_terms = NULL;
static size_t A396082_cnt = 0;
static size_t A396082_cap = 0;
static idx_t A396082_i = 0;       /* last processed index of A381466 */
static seq_t A396082_cur = 4;     /* A381466(A396082_i) */

static bool A396082_isprime(uint64_t p)
{
    if (p < 2) return false;
    if (p % 2 == 0) return p == 2;
    for (uint64_t d = 3; d * d <= p; d += 2)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    while (A396082_cnt < (size_t)n) {
        idx_t i = A396082_i + 1;
        seq_t prev = A396082_cur;
        seq_t ci = (seq_t)i;
        seq_t g = gcd(prev, ci);
        seq_t val;
        if (g == (seq_t)1)
            val = prev + ci;
        else
            val = ci / g;
        A396082_cur = val;
        A396082_i = i;
        if ((i & 1) == 0) {
            uint64_t p = (uint64_t)(i / 2);
            if (A396082_isprime(p) && val > ci) {
                if (A396082_cnt >= A396082_cap) {
                    size_t nc = A396082_cap ? A396082_cap * 2 : 1024;
                    seq_t *t = mem_resize(seq_t, A396082_terms, nc);
                    if (!t) abort();
                    A396082_terms = t;
                    A396082_cap = nc;
                }
                A396082_terms[A396082_cnt++] = (seq_t)p;
            }
        }
    }
    return A396082_terms[n - 1];
}
