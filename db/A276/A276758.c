/*
 * A276758  Numbers n such that A045876(n) = A045876(n+1).
 * https://oeis.org/A276758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 1 with A045876(k) = A045876(k+1), where
 *            A045876(k) = ((10^d-1)/9) * (digitsum(k) * N(k) / d), d =
 *            digits(k), N = number of distinct digit permutations; found by
 *            incremental search with memoization
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A045876_f(uint64_t m)
{
    static const uint64_t fact[21] = {
        1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL, 720ULL, 5040ULL, 40320ULL,
        362880ULL, 3628800ULL, 39916800ULL, 479001600ULL, 6227020800ULL,
        87178291200ULL, 1307674368000ULL, 20922789888000ULL,
        355687428096000ULL, 6402373705728000ULL, 121645100408832000ULL,
        2432902008176640000ULL
    };
    unsigned freq[10] = {0};
    uint64_t sum = 0, rep = 0;
    unsigned d = 0;
    do {
        unsigned dg = (unsigned)(m % 10);
        freq[dg]++;
        sum += dg;
        rep = rep * 10 + 1;
        m /= 10;
        d++;
    } while (m > 0);
    uint64_t N = fact[d];
    for (int i = 0; i < 10; i++)
        if (freq[i] > 1) N /= fact[freq[i]];
    return rep * (sum * N / d);
}

static uint64_t *A276758_terms = 0;
static uint64_t A276758_cnt = 0, A276758_cap = 0;
static uint64_t A276758_pos = 1;
static uint64_t A276758_fcur = 0;
static int A276758_init = 0;

seq_t a(idx_t n)
{
    if (!A276758_init) {
        A276758_fcur = A045876_f(1);
        A276758_init = 1;
    }
    while (A276758_cnt < (uint64_t)n) {
        uint64_t fn = A045876_f(A276758_pos + 1);
        if (fn == A276758_fcur) {
            if (A276758_cnt == A276758_cap) {
                A276758_cap = A276758_cap ? A276758_cap * 2 : 1024;
                A276758_terms = (uint64_t *)realloc(A276758_terms, A276758_cap * sizeof(uint64_t));
            }
            A276758_terms[A276758_cnt++] = A276758_pos;
        }
        A276758_fcur = fn;
        A276758_pos++;
    }
    return (seq_t)A276758_terms[n - 1];
}
