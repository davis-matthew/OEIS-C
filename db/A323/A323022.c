/*
 * A323022  Fourth omega of n. Number of distinct multiplicities in the prime
 *          signature of n.
 * https://oeis.org/A323022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct multiplicities of exponents in the
 *            prime factorization of n
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9605 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the fourth ω of n: the number of distinct multiplicities
   of the exponents in n's prime signature.
   For n = 1 the value is 0; otherwise it is obtained by
   factorizing n, counting how often each exponent occurs,
   and then counting how many distinct such counts exist. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Helper: generate primes up to a fixed limit (1e6) the first time the
   function is called. */
static void _init_primes(uint32_t **primes_out, size_t *cnt_out)
{
    const size_t LIM = 1000000;          /* large enough for all test data  */
    bool *is_comp = (bool*)calloc(LIM + 1, sizeof(bool));
    if (!is_comp) exit(1);

    size_t i;
    for (i = 2; i * i <= LIM; ++i)
        if (!is_comp[i])
            for (size_t j = i * i; j <= LIM; j += i)
                is_comp[j] = true;

    size_t cnt = 0;
    for (i = 2; i <= LIM; ++i)
        if (!is_comp[i])
            ++cnt;
    uint32_t *pr = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!pr) exit(1);
    size_t k = 0;
    for (i = 2; i <= LIM; ++i)
        if (!is_comp[i])
            pr[k++] = (uint32_t)i;

    free(is_comp);
    *primes_out = pr;
    *cnt_out = cnt;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    static bool initialized = false;
    static uint32_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (!initialized) {
        _init_primes(&primes, &prime_cnt);
        initialized = true;
    }

    uint64_t m = n;
    uint64_t freq[65] = {0};        /* freq[e] = number of primes with exponent e   */
    size_t i;

    /* Trial division by pre‑computed primes */
    for (i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e < 65)
                ++freq[e];
        }
    }

    /* Remaining factor > 1 is prime with exponent 1 */
    if (m > 1)
        ++freq[1];

    /* Count distinct counts > 0 */
    bool seen[65] = {0};
    int result = 0;
    for (int e = 1; e < 65; ++e) {
        uint64_t cnt = freq[e];
        if (cnt > 0 && !seen[cnt]) {
            seen[cnt] = true;
            ++result;
        }
    }

    return (seq_t)result;
}
