/*
 * A338610  Integers m such that there exist one prime p and one positive
 *          integer k, for which the expression k^3 + k^2*p is a perfect cube
 *          m^3.
 * https://oeis.org/A338610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A111251(n)+1)*(A111251(n))^2 (i.e. a(n) = ((q+1)*q*q)
 *            where q is the n‑th integer such that 3*q^2+3*q+1 is prime)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 402, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 9720 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n=0 */

    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;       /* allocated capacity */
    static size_t  count = 0;       /* number of terms stored */
    static uint64_t last_q = 0;     /* last q examined */

    while (count < n) {
        uint64_t q = last_q + 1;

        /* p = 3*q^2 + 3*q + 1 */
        __int128 p128 = (__int128)3 * q * q + (__int128)3 * q + 1;
        seq_t p = (seq_t)(uint64_t)p128;          /* p fits in 64 bits */

        if (isPrimeGeneric(p)) {
            /* m = (q+1)*q^2 */
            __int128 qsq = (__int128)q * q;
            seq_t m = (seq_t)(qsq * (q + 1));

            /* grow cache if necessary */
            if (count == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = m;
        }
        last_q = q;
    }

    return cache[n - 1];
}
