/*
 * A242094  Complement of A003249.
 * https://oeis.org/A242094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not in A003249, where A003249(n) =
 *            A001950(A003234(n)) + 1; computed by search/merge
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"

static uint64_t A003249_L(uint64_t m) { return (m + isqrt_u64(5 * m * m)) / 2; }
static uint64_t A003249_B(uint64_t m) { return A003249_L(m) + 2 * m; }
static uint64_t A003249_U(uint64_t m) { return A003249_L(m) + m; }

static uint64_t *A003249_list = NULL;
static idx_t A003249_cnt = 0, A003249_cap = 0;
static uint64_t A003249_next = 1;

static uint64_t A003249_a(idx_t n) {
    while (A003249_cnt < n) {
        uint64_t m = A003249_next++;
        if (A003249_U(A003249_B(m)) == A003249_B(A003249_U(m)) + 1) {
            if (A003249_cnt == A003249_cap) {
                idx_t nc = A003249_cap ? 2 * A003249_cap : 1024;
                uint64_t *p = (uint64_t *)realloc(A003249_list, (size_t)nc * sizeof(uint64_t));
                if (!p) return 0;
                A003249_list = p;
                A003249_cap = nc;
            }
            A003249_list[A003249_cnt++] = m;
        }
    }
    return A003249_U(A003249_list[n - 1]) + 1;
}

seq_t a(idx_t n) {
    idx_t count = 0, j = 1;
    uint64_t k = 0;
    uint64_t cur = A003249_a(1);
    while (1) {
        k++;
        while (cur < k) {
            j++;
            cur = A003249_a(j);
        }
        if (cur != k) {
            count++;
            if (count == n) return (seq_t)k;
        }
    }
}
