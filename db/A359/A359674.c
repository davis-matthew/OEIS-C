/*
 * A359674  Zero-based weighted sum of the prime indices of n in weakly
 *          increasing order.
 * https://oeis.org/A359674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..k} (i-1)*y_i * where y_i are the prime indices
 *            of n (with multiplicity), sorted. * * The function factorizes n,
 *            obtains for each prime factor p the index * m such that
 *            prime(m)=p, and accumulates the zero‑based weighted sum * using
 *            a closed form for repeated indices. A static sie...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"

static bool primes_inited = false;
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static const idx_t PRIMES_LIMIT = 100000;

static void init_primes(void) {
    if (!primes_inited) {
        build_primes(PRIMES_LIMIT, &primes, &primes_cnt);
        primes_inited = true;
    }
}

/* Binary search for the 1‑based index of prime value p
 * (p must be ≤ PRIMES_LIMIT and present in the primes array). */
static inline idx_t prime_index(uint64_t p) {
    idx_t left = 0, right = primes_cnt - 1;
    while (left <= right) {
        idx_t mid = (left + right) >> 1;
        if (primes[mid] == p)
            return mid + 1;            /* 1‑based index */
        if (primes[mid] < p)
            left = mid + 1;
        else
            right = (mid - 1);
    }
    return 0; /* should never happen */
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    init_primes();

    uint64_t temp = (uint64_t)n;
    idx_t pos = 1;      /* position in the sorted multiset (1‑based) */
    seq_t sum = 0;

    /* Trial division using the precomputed primes. */
    for (idx_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            uint64_t p = primes[i];
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            idx_t idx = i + 1;                     /* 1‑based prime index */
            seq_t w0 = (seq_t)(pos - 1);           /* current weight offset */
            seq_t contrib = (seq_t)idx *
                            ( (seq_t)e * w0 + (seq_t)e * (e - 1) / 2 );
            sum += contrib;
            pos += e;
        }
    }

    /* Remaining prime factor > sqrt(n), if any. */
    if (temp > 1) {
        idx_t idx = prime_index(temp);
        seq_t w0 = (seq_t)(pos - 1);
        sum += (seq_t)idx * w0;   /* e = 1 → e*(e-1)/2 = 0 */
    }

    return sum;
}
