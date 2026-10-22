/*
 * A334079  Total area of all triangles such that p + q = 2*n, p < q (p, q
 *          prime), with base (q - p) and height p.
 * https://oeis.org/A334079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{p<q, p+q=2n} (q-p)*p/2
 *          = Σ_{p prime, p<n} (2n-2p)*p/2, where q=2n-p is prime
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Local prime sieve cache */
static uint64_t sieve_limit = 0;
static bool *prime_flag = NULL;

/* Ensure sieve up to given limit */
static void ensure_sieve(uint64_t limit) {
    if (limit <= sieve_limit)
        return;
    if (prime_flag)
        free(prime_flag);
    prime_flag = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!prime_flag)
        exit(EXIT_FAILURE);
    for (uint64_t i = 0; i <= limit; ++i)
        prime_flag[i] = true;
    prime_flag[0] = prime_flag[1] = false;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                prime_flag[j] = false;
        }
    }
    sieve_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;
    uint64_t N = (uint64_t)n;
    uint64_t limit = 2 * N;
    ensure_sieve(limit);
    seq_t sum = (seq_t)0;
    for (uint64_t p = 2; p < N; ++p) {
        if (!prime_flag[p])
            continue;
        uint64_t q = 2 * N - p;
        if (q <= p)      /* ensures p < q */
            continue;
        if (!prime_flag[q])
            continue;
        seq_t area = ((seq_t)(q - p)) * ((seq_t)p);
        area /= (seq_t)2;
        sum += area;
    }
    return sum;
}
