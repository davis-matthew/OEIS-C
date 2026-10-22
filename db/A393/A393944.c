/*
 * A393944  Number of subsets of [n] in which exactly one third of the
 *          elements are primes and one third of the elements are squares.
 * https://oeis.org/A393944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..min(p,s,n-p-s)} C(p,i)*C(s,i)*C(n-p-s,i), where
 *            p = pi(n), s = floor(sqrt(n))
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 48, u64 n <= 109,
 *            u128 n <= 288, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------*/

#include <stdint.h>
#include <stdlib.h>

static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;  /* exact division */
    }
    return res;
}

static idx_t isqrt(idx_t n) {
    idx_t left = 0, right = n;
    while (left <= right) {
        idx_t mid = left + ((right - left) >> 1);
        if (mid * mid <= n) left = mid + 1;
        else right = mid - 1;
    }
    return right;
}

/*------------------------------------------------------------*/

seq_t a(idx_t n) {
    /* Static sieve cache */
    static uint8_t *prime_flag = NULL;
    static idx_t *pi_prefix = NULL;
    static idx_t max_n = 0;
    static int initialized = 0;

    if (!initialized || n > max_n) {
        size_t sz = (size_t)(n + 1);
        uint8_t *new_flag = (uint8_t *)realloc(prime_flag, sz);
        idx_t *new_prefix = (idx_t *)realloc(pi_prefix, sz * sizeof(idx_t));
        if (!new_flag || !new_prefix) exit(1); /* allocation failure */

        prime_flag = new_flag;
        pi_prefix = new_prefix;

        for (idx_t i = 0; i <= n; ++i)
            prime_flag[i] = 1;
        if (n >= 0) prime_flag[0] = 0;
        if (n >= 1) prime_flag[1] = 0;

        for (idx_t i = 2; i * i <= n; ++i) {
            if (prime_flag[i]) {
                for (idx_t j = i * i; j <= n; j += i)
                    prime_flag[j] = 0;
            }
        }

        pi_prefix[0] = 0;
        for (idx_t i = 1; i <= n; ++i)
            pi_prefix[i] = pi_prefix[i - 1] + prime_flag[i];

        max_n = n;
        initialized = 1;
    }

    idx_t p = pi_prefix[n];
    idx_t s = isqrt(n);
    idx_t remaining = n - p - s;
    idx_t m = p;
    if (s < m) m = s;
    if (remaining < m) m = remaining;
    if (m > n) m = n;   /* safety */

    seq_t result = 0;
    for (idx_t i = 0; i <= m; ++i) {
        seq_t term = binom(p, i);
        term = term * binom(s, i);
        term = term * binom(remaining, i);
        result += term;
    }
    return result;
}
