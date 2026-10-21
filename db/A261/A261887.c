/*
 * A261887  Number of triples of primes (p,q,r) that satisfy p+q^2+r^3=n.
 * https://oeis.org/A261887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples of primes (p,q,r) with p + q^2 + r^3 =
 *            n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                /* offset is 1 */
        return 0;

    /* ---- Sieve of Eratosthenes up to n --------------------------------- */
    size_t N = (size_t)n;
    bool *is_prime = (bool *)malloc((N + 1) * sizeof(bool));
    if (!is_prime)
        return 0;            /* memory failure */

    for (size_t i = 0; i <= N; ++i)
        is_prime[i] = true;
    if (N >= 0) is_prime[0] = false;
    if (N >= 1) is_prime[1] = false;

    for (size_t i = 2; (size_t)(i * i) <= N; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= N; j += i)
                is_prime[j] = false;
        }
    }

    /* ---- Collect all primes up to n ------------------------------------ */
    uint64_t *primes = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return 0;
    }
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (is_prime[i])
            primes[prime_cnt++] = (uint64_t)i;
    }

    /* ---- Pre‑compute squares and cubes of the primes --------------------- */
    uint64_t *squares = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    uint64_t *cubes   = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!squares || !cubes) {
        free(is_prime);
        free(primes);
        free(squares);
        free(cubes);
        return 0;
    }

    size_t sq_cnt = 0, cube_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t sq = p * p;
        if (sq <= n)
            squares[sq_cnt++] = sq;
        uint64_t cb = p * p * p;
        if (cb <= n)
            cubes[cube_cnt++] = cb;
    }

    /* ---- Count admissible triples --------------------------------------- */
    seq_t count = 0;
    for (size_t ci = 0; ci < cube_cnt; ++ci) {
        seq_t rem_after_cube = (seq_t)n - (seq_t)cubes[ci];
        for (size_t si = 0; si < sq_cnt; ++si) {
            seq_t sq_val = (seq_t)squares[si];
            if (sq_val > rem_after_cube)
                break;
            seq_t p_val = rem_after_cube - sq_val;
            if (p_val >= 2 && is_prime[(size_t)p_val])
                ++count;
        }
    }

    /* ---- Clean up ------------------------------------------------------- */
    free(is_prime);
    free(primes);
    free(squares);
    free(cubes);

    return count;
}
