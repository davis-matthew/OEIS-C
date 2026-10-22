/*
 * A347563  Binomial complement triangle, T(n,k) = LCM(1,...,n)/binomial(n,k)
 *          for 0 <= k <= n, a(0) = T(0,0) = 0, read by rows.
 * https://oeis.org/A347563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where T(r,k) = LCM(1,…,r)/C(r,k) and (r,k) is the
 *            row/column position of the n‑th term in the row‑wise ordering of
 *            the Binomial complement triangle. a(0)=0
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Small prime table, extended when needed. */
static unsigned int primes[200000];
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* Generate all primes ≤ limit using trial division, adding to the static table. */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit) return;
    for (idx_t n = primes_limit + 1; n <= limit; ++n) {
        if (n < 2) continue;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            unsigned int p = primes[i];
            if ((uint64_t)p * (uint64_t)p > n) break;
            if (n % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt < sizeof(primes)/sizeof(primes[0]))
                primes[primes_cnt++] = (unsigned int)n;
        }
    }
    primes_limit = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) return (seq_t)0;

    /* Find row r such that terms up to row r (inclusive) contain index n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    idx_t terms_before = r * (r + 1) / 2;
    idx_t k = n - terms_before;      /* column index in row r */

    /* Ensure we have all primes up to r. */
    ensure_primes(r);

    seq_t result = (seq_t)1;

    /* Compute the contribution of each prime. */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        unsigned int p_val = primes[pi];
        if ((idx_t)p_val > r) break;

        /* Exponent of p in LCM(1..r) = floor(log_p(r)). */
        idx_t exp_lcm = 0;
        uint64_t power = p_val;
        while (power <= (uint64_t)r) {
            ++exp_lcm;
            if (power > (uint64_t)r / p_val) break;
            power *= p_val;
        }

        /* Exponent of p in binomial(r,k). */
        idx_t exp_binom = 0;
        power = p_val;
        while (power <= (uint64_t)r) {
            exp_binom += r / power - k / power - (r - k) / power;
            if (power > (uint64_t)r / p_val) break;
            power *= p_val;
        }

        idx_t exp_diff = exp_lcm - exp_binom;
        if (exp_diff == 0) continue;

        /* Multiply result by p^exp_diff. */
        seq_t mult = (seq_t)1;
        for (idx_t i = 0; i < exp_diff; ++i) mult *= (seq_t)p_val;
        result *= mult;
    }

    return result;
}
