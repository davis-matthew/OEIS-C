/*
 * A338826  G.f.: (1/(1 + x)) * Product_{k>=1} 1/(1 + x^prime(k)).
 * https://oeis.org/A338826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in (1/(1+x)) * ∏_{p prime} 1/(1+x^p)
 *            a(n) equals the difference between the number of partitions of n
 *            into an even number of prime parts (including 1) and the number
 *            of partitions into an odd number of prime parts (including 1)
 * Range:     i8 n <= 31, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n) {
    idx_t N = n;

    /* ---------- Prime table (static, rebuilt only when N grows) ---------- */
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;          /* largest N for which we have primes */

    if (prime_limit < N) {
        /* Rebuild primes up to N */
        if (prime_list) {
            free(prime_list);
            prime_list = NULL;
            prime_count = 0;
        }
        size_t lim = (size_t)N;
        char *is_prime = (char*)malloc((lim + 1) * sizeof(char));
        if (!is_prime) return 0;          /* allocation failure fallback */
        for (size_t i = 0; i <= lim; ++i) is_prime[i] = 1;
        if (lim >= 0) is_prime[0] = 0;
        if (lim >= 1) is_prime[1] = 0;

        for (size_t i = 2; i * i <= lim; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= lim; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Count primes */
        size_t cnt = 0;
        for (size_t i = 2; i <= lim; ++i)
            if (is_prime[i]) ++cnt;

        prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
        size_t idx = 0;
        for (size_t i = 2; i <= lim; ++i)
            if (is_prime[i]) prime_list[idx++] = (uint64_t)i;

        prime_count = cnt;
        prime_limit = N;
        free(is_prime);
    }

    /* ---------- Coefficient array (coefficients of the generating function) ---------- */
    seq_t *seq = (seq_t*)calloc((size_t)N + 1, sizeof(seq_t));
    if (!seq) return 0;                  /* allocation failure fallback */

    seq[0] = 1;                          /* by convention 1/(1+1) contributes +1 */

    /* Process coin 1 (part 1 is treated like any other part) */
    for (idx_t s = 1; s <= N; ++s) {
        seq[s] -= seq[s - 1];             /* coefficient update for 1/(1+x) */
    }

    /* Process prime coins */
    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint64_t p = prime_list[pi];
        if (p > (uint64_t)N) break;
        idx_t cp = (idx_t)p;
        for (idx_t s = cp; s <= N; ++s) {
            seq[s] -= seq[s - cp];        /* update for 1/(1+x^p) */
        }
    }

    seq_t result = seq[N];
    free(seq);
    return result;
}
