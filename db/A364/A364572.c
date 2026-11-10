/*
 * A364572  Starting from k=7, each subsequent term is the next larger odd k
 *          such that A005940(k) >= k and the ratio A005940(k)/k is nearer to
 *          1.0 than for any previous k in the sequence.
 * https://oeis.org/A364572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=7; a(n) = next odd k > a(n-1) with A005940(k) >= k and
 *            (A005940(k)-k)/k < (A005940(a(n-1))-a(n-1))/a(n-1); found by
 *            incremental search over odd k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int A364572_doudna(uint64_t k, uint64_t *out) {
    static const uint64_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
        59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131
    };
    uint64_t m = k - 1;
    uint64_t result = 1;
    unsigned i = 1;
    while (m > 0) {
        if (m & 1) {
            uint64_t p = primes[i - 1];
            if (result > UINT64_MAX / p) return 0;
            result *= p;
            m = (m - 1) >> 1;
        } else {
            m >>= 1;
            i++;
        }
    }
    *out = result;
    return 1;
}

#define A364572_MAX 4096

seq_t a(idx_t n) {
    static uint64_t terms[A364572_MAX];
    static size_t count = 0;
    static uint64_t next_k = 7;
    static uint64_t best_diff = 0; /* A(k)-k of the latest term */
    static uint64_t best_k = 0;

    if (n < 1 || n > A364572_MAX) return 0;
    while (count < (size_t)n) {
        uint64_t k = next_k;
        next_k += 2;
        uint64_t A;
        if (!A364572_doudna(k, &A)) continue;
        if (A < k) continue;
        uint64_t d = A - k;
        if (count == 0) {
            terms[count++] = k;
            best_diff = d;
            best_k = k;
            continue;
        }
        /* d/k < best_diff/best_k */
        unsigned __int128 lhs = (unsigned __int128)d * best_k;
        unsigned __int128 rhs = (unsigned __int128)best_diff * k;
        if (lhs < rhs) {
            terms[count++] = k;
            best_diff = d;
            best_k = k;
        }
    }
    return (seq_t)terms[n - 1];
}
