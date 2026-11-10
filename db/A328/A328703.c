/*
 * A328703  Numbers k dividing nonzero terms in A002065.
 * https://oeis.org/A328703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that the orbit of 0 under x -> x^2+x+1
 *            (mod k) returns to 0 (k divides A002065(m) for some m>0); found
 *            by search
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A328703_terms = 0;
static uint64_t A328703_count = 0;
static uint64_t A328703_cap = 0;
static uint64_t A328703_next = 1;

static int A328703_ok(uint64_t k)
{
    if (k == 1) return 1;
    if ((k & 1) == 0) return 0; /* iterates are odd after step 1 */
    uint64_t x = 0;
    for (uint64_t i = 0; i < k; i++) {
        x = (x * x + x + 1) % k;
        if (x == 0) return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    while (A328703_count < (uint64_t)n) {
        uint64_t k = A328703_next++;
        if (A328703_ok(k)) {
            if (A328703_count == A328703_cap) {
                uint64_t nc = A328703_cap ? A328703_cap * 2 : 64;
                uint64_t *p = (uint64_t *)realloc(A328703_terms, nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                A328703_terms = p;
                A328703_cap = nc;
            }
            A328703_terms[A328703_count++] = k;
        }
    }
    return (seq_t)A328703_terms[n - 1];
}
