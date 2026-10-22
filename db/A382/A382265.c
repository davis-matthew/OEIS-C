/*
 * A382265  In the prime factorization of n replace the k-th prime with the
 *          k-th nonprime number.
 * https://oeis.org/A382265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 6560, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A382265 – replace the k-th prime in the factorisation of n
   with the k-th non‑prime number.
   a(n) = ∏ nonprime(k)^e(k)  where
   n = ∏ prime(k)^e(k)  (prime and nonprime indices start at 1). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"
#include "../utils/build_primes.h"

/* External-linkage function required by the test harness */
seq_t a(idx_t n)
{
    /* ---------- static tables for primes / non‑primes --------- */
    static struct {
        bool          init;          /* initialise once */
        uint64_t     *primes;        /* array of primes                 */
        size_t        prime_cnt;     /* number of primes                 */
        uint64_t     *nonprimes;     /* first prime_cnt non‑primes      */
        size_t        nonprime_cnt;  /* number of stored non‑primes     */
    } data = { false, NULL, 0, NULL, 0 };

    /* initialise tables only once (enough for n ≤ 10000) */
    if (!data.init) {
        data.init = true;
        const idx_t limit = 20000;           /* sufficient for 10 000 */
        /* primes up to limit */
        build_primes(limit, &data.primes, &data.prime_cnt);

        /* allocate array for non‑prime values */
        data.nonprimes = (uint64_t *)malloc(data.prime_cnt * sizeof(uint64_t));
        size_t cnt = 0;
        for (uint64_t x = 1; x <= limit && cnt < data.prime_cnt; ++x) {
            if (!is_prime_small((seq_t)x)) {
                data.nonprimes[cnt] = x;
                ++cnt;
            }
        }
        data.nonprime_cnt = cnt;
    }

    /* a(1) = 1 (empty product) */
    if (n <= 1U)
        return (seq_t)1;

    /* ---------- factorise n using the prime table ---------- */
    uint64_t pfactors[32];
    uint64_t exponents[32];
    size_t   pf_cnt = 0;
    factor_u64((uint64_t)n, pfactors, exponents, &pf_cnt);

    /* ---------- replace each prime by corresponding non‑prime ----- */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t prime_val   = pfactors[i];
        uint64_t nonprime_val;

        /* find index of prime_val among the stored primes */
        size_t lo = 0, hi = data.prime_cnt;
        size_t idx = data.prime_cnt;          /* sentinel */
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (data.primes[mid] < prime_val)
                lo = mid + 1;
            else if (data.primes[mid] > prime_val)
                hi = mid;
            else {
                idx = mid;
                break;
            }
        }

        /* use the twin non‑prime if available, otherwise keep prime */
        if (idx < data.nonprime_cnt)
            nonprime_val = data.nonprimes[idx];
        else
            nonprime_val = prime_val; /* shouldn't happen for n ≤ 10000 */

        /* multiply by nonprime_val^exponents[i] */
        for (uint64_t e = 0; e < exponents[i]; ++e)
            result *= (seq_t)nonprime_val;
    }

    return result;
}
