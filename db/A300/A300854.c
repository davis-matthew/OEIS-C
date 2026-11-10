/*
 * A300854  a(n) is the smallest prime p = prime(k) such that A300845(k) =
 *          prime(n), or 0 if no such k exists.
 * https://oeis.org/A300854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p = prime(k) such that A300845(k) = prime(n),
 *            else 0; found by scanning primes p in increasing order
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A300854_q = NULL;     /* list of small primes q */
static uint64_t *A300854_first = NULL; /* first p giving q, 0 if none yet */
static size_t A300854_cnt = 0, A300854_cap = 0;
static uint64_t A300854_cur = 1;       /* last prime p processed */

static uint64_t A300854_qat(size_t i)
{
    while (i >= A300854_cnt) {
        if (A300854_cnt == A300854_cap) {
            size_t nc = A300854_cap ? A300854_cap * 2 : 64;
            A300854_q = (uint64_t *)realloc(A300854_q, nc * sizeof(uint64_t));
            A300854_first = (uint64_t *)realloc(A300854_first, nc * sizeof(uint64_t));
            for (size_t j = A300854_cap; j < nc; j++) A300854_first[j] = 0;
            A300854_cap = nc;
        }
        uint64_t c = A300854_cnt ? A300854_q[A300854_cnt - 1] + (A300854_q[A300854_cnt - 1] == 2 ? 1 : 2) : 2;
        while (!is_prime_u64(c)) c += 2;
        A300854_q[A300854_cnt++] = c;
    }
    return A300854_q[i];
}

static void A300854_step(void)
{
    uint64_t p = A300854_cur + (A300854_cur < 2 ? 1 : (A300854_cur == 2 ? 1 : 2));
    while (!is_prime_u64(p)) p += 2;
    A300854_cur = p;
    for (size_t i = 0;; i++) {
        uint64_t q = A300854_qat(i);
        uint64_t v = q * q + q * p + p * p;
        if (is_prime_u64(v)) {
            if (A300854_first[i] == 0) A300854_first[i] = p;
            return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t i = (size_t)(n - 1);
    A300854_qat(i);
    while (A300854_first[i] == 0 && A300854_cur < 2000000000ULL)
        A300854_step();
    return (seq_t)A300854_first[i];
}
