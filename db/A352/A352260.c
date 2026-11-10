/*
 * A352260  Integers that need 2 iterations of the map x->A352172(x) to reach
 *          1.
 * https://oeis.org/A352260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer m with A352172(m) != 1 and
 *            A352172(A352172(m)) = 1; equivalently the nonzero digits of m
 *            lie in {1,2,4,5,8} and the exponent of 2 in their product equals
 *            the number of 5s, and is at least 1 (A352172(m) = 1000^k).
 *            Computed by incremental search with a cache
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A352260_ok(uint64_t m) {
    int twos = 0, fives = 0;
    while (m > 0) {
        unsigned d = (unsigned)(m % 10);
        m /= 10;
        switch (d) {
            case 0: case 1: break;
            case 2: twos += 1; break;
            case 4: twos += 2; break;
            case 8: twos += 3; break;
            case 5: fives += 1; break;
            default: return false;
        }
    }
    return twos == fives && twos > 0;
}

static uint64_t *A352260_terms = 0;
static size_t A352260_cnt = 0, A352260_cap = 0;
static uint64_t A352260_next = 1;

seq_t a(idx_t n) {
    while ((idx_t)A352260_cnt < n) {
        uint64_t m = A352260_next++;
        if (A352260_ok(m)) {
            if (A352260_cnt == A352260_cap) {
                size_t nc = A352260_cap ? A352260_cap * 2 : 256;
                A352260_terms = (uint64_t *)realloc(A352260_terms, nc * sizeof(uint64_t));
                A352260_cap = nc;
            }
            A352260_terms[A352260_cnt++] = m;
        }
    }
    return (seq_t)A352260_terms[n - 1];
}
