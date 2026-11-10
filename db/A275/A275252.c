/*
 * A275252  Sequence of pairwise relatively prime numbers of class P_7 (see
 *          comment in A275246).
 * https://oeis.org/A275252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=16; for n>=2, with m=floor((n-2)/4), j=(n-2) mod 4 and
 *            b=2+8m, a(n) = prime(b+u_j)*prime(b+v_j), where (u_j,v_j) =
 *            (0,3),(1,5),(2,6),(4,7)
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-08)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *A275252_primes = NULL;
static size_t A275252_cnt = 0;
static size_t A275252_limit = 0;

static void A275252_ensure(size_t need)
{
    if (A275252_cnt >= need)
        return;
    size_t limit = A275252_limit > 0 ? A275252_limit : 1000;
    for (;;) {
        uint64_t *tmp = NULL;
        size_t cnt = 0;
        build_primes_upto((idx_t)limit, &tmp, &cnt);
        if (cnt >= need) {
            free(A275252_primes);
            A275252_primes = tmp;
            A275252_cnt = cnt;
            A275252_limit = limit;
            break;
        }
        free(tmp);
        if (limit < 16)
            limit = 16;
        else
            limit <<= 1;
    }
}

seq_t a(idx_t n)
{
    static const int U[4] = {0, 1, 2, 4};
    static const int V[4] = {3, 5, 6, 7};
    if (n == 1)
        return (seq_t)16;

    idx_t m = (n - 2) / 4;
    int j = (int)((n - 2) % 4);
    size_t base = (size_t)(2 + 8 * m);
    size_t i1 = base + (size_t)U[j];
    size_t i2 = base + (size_t)V[j];

    if (i2 > A275252_cnt)
        A275252_ensure(i2);

    seq_t v1 = (seq_t)A275252_primes[i1 - 1];
    seq_t v2 = (seq_t)A275252_primes[i2 - 1];
    return v1 * v2;
}
