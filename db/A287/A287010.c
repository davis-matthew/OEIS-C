/*
 * A287010  Triangle T(n,m): floor(log(A002110(n))/log(prime(m))).
 * https://oeis.org/A287010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = ⌊∑_{i=1}^{r} log(p_i) / log(p_k)⌋, where r is the row
 *            containing the n‑th flattened term and k is the term’s position
 *            in that row (1‑based)
 * Range:     u8 n <= 946, u16 n <= 9525, u32 n <= 9525, u64 n <= 9525,
 *            u128 n <= 9525, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static data, initialized once */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static long double *log_primes = NULL;
    static long double *cum_log = NULL;

    if (!initialized) {
        /* We only need enough primes for n up to about 10 000.
           The 12 000‑th prime is < 140 000, so we sieve up to 150 000. */
        idx_t sieve_limit = 150000;
        build_primes_upto(sieve_limit, &primes, &prime_cnt);

        /* Pre‑compute log₂ of every prime and its cumulative sum. */
        log_primes = (long double *)malloc(prime_cnt * sizeof(long double));
        cum_log = (long double *)malloc((prime_cnt + 1) * sizeof(long double));
        cum_log[0] = 0.0L;
        for (size_t i = 0; i < prime_cnt; ++i) {
            log_primes[i] = log2l((long double)primes[i]);
            cum_log[i + 1] = cum_log[i] + log_primes[i];
        }
        initialized = true;
    }

    /* Find the row r that contains the n‑th flattened term.  */
    long double tmp = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((tmp - 1.0L) / 2.0L);          /* floor */
    /* If we undershoot, bump up.  */
    unsigned __int128 terms_before = (unsigned __int128)r * (r + 1) / 2;
    if (terms_before < n) ++r;

    /* Position k within the row (1‑based).  */
    unsigned __int128 prev_terms = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev_terms);             /* 1 ≤ k ≤ r */

    /* Compute the exponent using high‑precision long double logs. */
    long double sum_logs   = cum_log[r];
    long double prime_log  = log_primes[k - 1];
    long double quotient   = sum_logs / prime_log;

    /* Small epsilon guards against round‑off that could push the
       floor value low.  */
    const long double eps   = 1e-12L;
    seq_t result = (seq_t)((unsigned long long)floorl(quotient + eps));
    return result;
}
