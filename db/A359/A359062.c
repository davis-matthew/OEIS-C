/*
 * A359062  Nonprime terms of A359059.
 * https://oeis.org/A359062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite-or-1 number k with 3 |
 *            (phi(k)+rad(k)+psi(k)); found by a sequential search (primes
 *            always satisfy the condition and are skipped)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-10)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

static unsigned A359062_pow_mod3(unsigned base, unsigned e)
{
    unsigned res = 1;
    unsigned b = base % 3;
    while (e)
    {
        if (e & 1U)
            res = (res * b) % 3;
        b = (b * b) % 3;
        e >>= 1U;
    }
    return res;
}

/* returns true if k is a term: nonprime and 3 | phi+rad+psi */
static bool A359062_test(uint64_t k)
{
    if (k == 1)
        return true;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);
    if (cnt == 1 && exps[0] == 1)
        return false; /* prime */
    unsigned phi = 1, rad = 1, psi = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        unsigned pm = (unsigned)(primes[i] % 3);
        unsigned pw = A359062_pow_mod3(pm, (unsigned)(exps[i] - 1U));
        phi = (phi * (((pm + 2U) % 3) * pw % 3)) % 3;
        rad = (rad * pm) % 3;
        psi = (psi * (((pm + 1U) % 3) * pw % 3)) % 3;
    }
    return (phi + rad + psi) % 3U == 0U;
}

static seq_t *A359062_cache = NULL;
static size_t A359062_len = 0;
static size_t A359062_cap = 0;
static uint64_t A359062_next = 1;

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    while ((size_t)n > A359062_len)
    {
        if (A359062_test(A359062_next))
        {
            if (A359062_len >= A359062_cap)
            {
                size_t nc = A359062_cap == 0 ? 64 : A359062_cap * 2;
                A359062_cache = mem_resize(seq_t, A359062_cache, nc);
                A359062_cap = nc;
            }
            A359062_cache[A359062_len++] = (seq_t)A359062_next;
        }
        ++A359062_next;
    }
    return A359062_cache[n - 1];
}
