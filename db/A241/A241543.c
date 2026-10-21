/*
 * A241543  a(n) = A241477(n, n).
 * https://oeis.org/A241543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor(n/2)*(n-2)!/(floor(n/2)!^2) for n>=2, a(0)=1,
 *            a(1)=1
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t _pow_uint(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 2) return 1;                 /* a(0) = a(1) = 1 */

    idx_t k = n / 2;                     /* floor(n/2) */
    idx_t m = n - 2;                     /* n-2 */

    /* Build prime table up to max(m,2) */
    idx_t limit = m;
    if (limit < 2) limit = 2;

    size_t sz = (size_t)(limit + 1);
    char *is_comp = (char *)malloc(sz);
    if (!is_comp) return 0;              /* allocation failure, return 0 */
    for (size_t i = 0; i < sz; ++i) is_comp[i] = 0;

    size_t primes_alloc = limit / 10 + 10;
    if (primes_alloc < 10) primes_alloc = 10;
    size_t primes_cnt = 0;
    uint32_t *primes = (uint32_t *)malloc(primes_alloc * sizeof(uint32_t));
    if (!primes) { free(is_comp); return 0; }

    for (uint32_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            if (primes_cnt == primes_alloc) {
                primes_alloc <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_alloc * sizeof(uint32_t));
                if (!tmp) { free(is_comp); free(primes); return 0; }
                primes = tmp;
            }
            primes[primes_cnt++] = p;
            if ((uint64_t)p * p <= limit) {
                for (uint32_t mult = p * p; mult <= limit; mult += p) {
                    is_comp[mult] = 1;
                }
            }
        }
    }

    seq_t result = 1;

    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint32_t p = primes[idx];

        /* exponent of p in m! */
        idx_t e_num = 0, pp = p;
        while (pp <= m) {
            e_num += m / pp;
            if (pp > m / p) break;
            pp *= p;
        }

        /* exponent of p in k! */
        idx_t e_k = 0; pp = p;
        while (pp <= k) {
            e_k += k / pp;
            if (pp > k / p) break;
            pp *= p;
        }

        /* exponent of p in the integer k (not k!) */
        idx_t e_k_val = 0;
        uint64_t tmp_k = k;
        while (tmp_k % p == 0) {
            tmp_k /= p;
            ++e_k_val;
        }

        /* extra factor from 2*k */
        idx_t e_extra = (p == 2) ? (1 + e_k_val) : e_k_val;

        /* final exponent in the result */
        idx_t exp = e_num + e_extra - 2 * e_k;
        if (exp > 0) {
            seq_t pow_val = _pow_uint((seq_t)p, exp);
            result *= pow_val;
        }
    }

    free(is_comp);
    free(primes);
    return result;
}
