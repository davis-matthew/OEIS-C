/*
 * A276075  a(1) = 0, a(n) = (e1*i1! + e2*i2! + ... + ez*iz!) for n =
 *          prime(i1)^e1 * prime(i2)^e2 * ... * prime(iz)^ez, where prime(k)
 *          is the k-th prime, A000040(k).
 * https://oeis.org/A276075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p^e||n} e * (PrimePi(p))!
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 72, u128 n <= 148,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/factorial.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    /* Static cache of primes up to a fixed limit (enough for n <= 10000). */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static bool primes_ready = false;

    if (!primes_ready) {
        /* 100000 is comfortably larger than any prime factor of n <= 10000. */
        build_primes_upto(100000, &primes, &prime_cnt);
        primes_ready = true;
    }

    /* Factor n.  We use a small stack-allocated array for primes and exponents. */
    uint64_t p_arr[64];
    uint64_t e_arr[64];
    size_t fcnt = 0;
    factor_u64(n, p_arr, e_arr, &fcnt);

    seq_t result = (seq_t)0;

    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p = p_arr[i];
        uint64_t e = e_arr[i];

        /* Find the index of p in the primes array (1‑based). */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (primes[mid] < p)
                lo = mid + 1;
            else
                hi = mid;
        }
        /* lo is now the position of p (0‑based). */
        size_t prime_index = lo + 1;  /* convert to 1‑based index */

        seq_t fac = factorial((idx_t)prime_index);
        result += fac * (seq_t)e;
    }

    return result;
}
