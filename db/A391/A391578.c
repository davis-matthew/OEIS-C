/*
 * A391578  Numbers k for which A391573(k) < k.
 * https://oeis.org/A391578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: a(n) = n-th m >= 1 with A391573(m) < m, where A391573(m)
 *            = A048720(m,k)/k for the smallest odd k>1 with k | A048720(m,k)
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9948 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A391578_prop(uint64_t m)
{
    for (uint64_t k = 3; ; k += 2) {
        unsigned __int128 prod = 0;
        uint64_t kk = k;
        int sh = 0;
        while (kk) {
            if (kk & 1ULL) prod ^= ((unsigned __int128)m << sh);
            kk >>= 1;
            sh++;
        }
        if (prod % k == 0) {
            unsigned __int128 q = prod / k;
            return q < (unsigned __int128)m;
        }
    }
}

static uint64_t *A391578_terms = 0;
static uint64_t A391578_count = 0;
static uint64_t A391578_cap = 0;
static uint64_t A391578_next = 1;

seq_t a(idx_t n)
{
    while (A391578_count < (uint64_t)n) {
        if (A391578_prop(A391578_next)) {
            if (A391578_count == A391578_cap) {
                A391578_cap = A391578_cap ? A391578_cap * 2 : 1024;
                A391578_terms = (uint64_t *)realloc(A391578_terms, A391578_cap * sizeof(uint64_t));
            }
            A391578_terms[A391578_count++] = A391578_next;
        }
        A391578_next++;
    }
    return (seq_t)A391578_terms[n - 1];
}
