/*
 * A400979  Maximum of r + s - n over positive integers r and s such that
 *          r!*s! divides n!.
 * https://oeis.org/A400979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = maximum of r + s - n over positive integers r, s such
 *            that r! * s! divides n!
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper function: simple sieve to generate primes up to limit (inclusive) */
static void sieve_primes(idx_t limit, uint32_t **primes_ptr, size_t *cnt_ptr)
{
    if (limit < 2) {
        *primes_ptr = NULL;
        *cnt_ptr   = 0;
        return;
    }
    size_t size = limit + 1;
    uint8_t *is_prime = (uint8_t *)malloc(size);
    memset(is_prime, 1, size);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t q = p * p; q <= limit; q += p)
                is_prime[q] = 0;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *primes = NULL;
    if (cnt > 0) {
        primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
        size_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i])
                primes[idx++] = (uint32_t)i;
    }

    free(is_prime);
    *primes_ptr = primes;
    *cnt_ptr    = cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0)
        return (seq_t)2;

    /* --- Step 1: generate primes up to n --- */
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(n, &primes, &prime_cnt);

    /* Edge: if n == 1, no primes but still need to process r=1 */
    if (prime_cnt == 0) {
        /* Only r = s = 1 works */
        free(primes);
        return (seq_t)1;
    }

    /* --- Step 2: precompute exponent array exps[prime_idx][k] = e_p(k!) --- */
    /* Allocate [prime_cnt][n+1] array of uint16_t */
    uint16_t **exps = (uint16_t **)malloc(prime_cnt * sizeof(uint16_t *));
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        exps[pi] = (uint16_t *)malloc((n + 1) * sizeof(uint16_t));
        exps[pi][0] = 0;
        for (idx_t k = 1; k <= n; ++k) {
            uint32_t p = primes[pi];
            idx_t t = k;
            uint16_t cnt = 0;
            while (t % p == 0) {
                ++cnt;
                t /= p;
            }
            exps[pi][k] = exps[pi][k - 1] + cnt;
        }
    }

    /* --- Step 3: for each prime, build inverse table: for each exponent rem
       return largest s with exponent <= rem. --- */
    /* target[pi] = e_p(n!) */
    uint16_t *target = (uint16_t *)malloc(prime_cnt * sizeof(uint16_t));
    for (size_t pi = 0; pi < prime_cnt; ++pi)
        target[pi] = exps[pi][n];

    /* Build map arrays */
    uint16_t **max_s = (uint16_t **)malloc(prime_cnt * sizeof(uint16_t *));
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint16_t max_e = target[pi];
        max_s[pi] = (uint16_t *)malloc((max_e + 1) * sizeof(uint16_t));
        /* initialize with zeros */
        memset(max_s[pi], 0, (max_e + 1) * sizeof(uint16_t));
        for (idx_t k = 0; k <= n; ++k) {
            uint16_t e = exps[pi][k];
            if (e <= max_e && max_s[pi][e] < (uint16_t)k)
                max_s[pi][e] = (uint16_t)k;   // store largest k for exact exponent
        }
        /* propagate to ensure nondecreasing: largest s for any rem <= e */
        for (uint16_t e = 1; e <= max_e; ++e)
            if (max_s[pi][e] < max_s[pi][e - 1])
                max_s[pi][e] = max_s[pi][e - 1];
    }

    /* --- Step 4: compute maximum r + s - n --- */
    seq_t best = (seq_t)0;

    for (idx_t r = 1; r <= n; ++r) {
        /* Determine maximum s allowed by each prime */
        idx_t smax = n; /* upper bound */
        int possible = 1;
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            int rem = (int)target[pi] - (int)exps[pi][r];
            if (rem < 0) {
                possible = 0;
                break;
            }
            idx_t sp = max_s[pi][rem];
            if (sp < smax)
                smax = sp;
        }
        if (!possible || smax == 0)
            continue;          /* no feasible s for this r */
        seq_t cand = (seq_t)r + (seq_t)smax - (seq_t)n;
        if (cand > best)
            best = cand;
    }

    /* --- cleanup --- */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        free(exps[pi]);
        free(max_s[pi]);
    }
    free(exps);
    free(max_s);
    free(target);
    free(primes);

    return best;
}
