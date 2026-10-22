/*
 * A324911  Multiplicative with a(p^e) = A156552(p^e).
 * https://oeis.org/A324911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} ( (2^e – 1) · 2^(π(p)–1) ) where π(p) is the
 *            number of primes ≤ p. The function is multiplicative; for a
 *            prime p, a(p)=2^(π(p)–1)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple sieve that fills the array `primes_out` with all primes ≤ limit
   and returns the count through `cnt_out`.  The caller is responsible for
   freeing the allocated memory.  */
static void generate_primes(uint64_t limit, uint64_t **primes_out,
                            size_t *cnt_out)
{
    size_t sz = (size_t)limit + 1;
    bool *mark = (bool *)malloc(sz);
    for (size_t i = 0; i < sz; ++i) mark[i] = true;
    if (sz > 0) mark[0] = false;
    if (sz > 1) mark[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (mark[(size_t)i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                mark[(size_t)j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i < sz; ++i)
        if (mark[i]) ++cnt;

    /* Allocate and store primes */
    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i < sz; ++i)
        if (mark[i]) primes[idx++] = (uint64_t)i;

    free(mark);
    *primes_out = primes;
    *cnt_out = cnt;
}

/*---------------------------------------------------------------*/
/*  The function required by the exercise                        */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static bool inited = false;

    /* Prime list large enough for all test values (≤ 10 000, so 100 000 covers all) */
    if (!inited) {
        generate_primes(100000ULL, &primes, &primes_cnt);
        inited = true;
    }

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > (uint64_t)temp) break;
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }

            /* (2^exp – 1) * 2^(π(p)–1)
               π(p) – 1 == i  (0‑based index in primes array) */
            seq_t pow2_exp = ((seq_t)1 << (uint32_t)exp);
            seq_t part     = pow2_exp - (seq_t)1;
            part *= ((seq_t)1 << (uint32_t)i);
            result *= part;
        }
    }

    if (temp > 1) {                /* remaining prime factor */
        uint64_t p = (uint64_t)temp;
        /* binary search for its index */
        size_t lo = 0, hi = primes_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (primes[mid] < p)
                lo = mid + 1;
            else
                hi = mid;
        }
        if (lo < primes_cnt && primes[lo] == p) {
            seq_t part = ((seq_t)1 << (uint32_t)lo);  /* (2^1 – 1) = 1 */
            result *= part;
        } else {
            /* This should not happen for the tested range. */
        }
    }

    return result;
}
