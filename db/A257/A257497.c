/*
 * A257497  Number of ordered ways to write n as the sum of a term of A257121
 *          and a positive generalized pentagonal number.
 * https://oeis.org/A257497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(t,g) | t∈A257121, g>0, t+g=n}
 * Range:     u8 n <= 8573, u16 n <= 8573, u32 n <= 8573, u64 n <= 8573,
 *            u128 n <= 8573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257121 consists of the distinct values floor(m/3) such that 3m±1 are simultaneously prime. *
 * We compute all such t ≤ n-1, all positive generalized pentagonal numbers g ≤ n-1,
 * and count the pairs with t+g=n.         */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;                      /* offset starts at 1 */

    /*--------------------------------------------------------------*/
    /* 1.  Sieve primes up to 9*n (enough to test 3m±1 for m≤3n-1). */
    /*--------------------------------------------------------------*/
    idx_t maxP = 9 * n + 10;                  /* safety margin */
    size_t prime_size = (size_t)maxP + 1;
    bool *prime = (bool*)malloc(prime_size * sizeof(bool));
    if (!prime) return 0;
    prime[0] = prime[1] = false;
    for (size_t i = 2; i < prime_size; ++i) prime[i] = true;

    for (size_t i = 2; i * i <= maxP; ++i) {
        if (prime[i]) {
            for (size_t j = i * i; j <= (size_t)maxP; j += i)
                prime[j] = false;
        }
    }

    /*--------------------------------------------------------------*/
    /* 2.  Precompute positive generalized pentagonal numbers ≤ n.  */
    /*--------------------------------------------------------------*/
    size_t gp_size = (size_t)n + 1;
    unsigned char *is_gp = (unsigned char*)calloc(gp_size, sizeof(unsigned char));
    if (!is_gp) {
        free(prime);
        return 0;
    }
    for (idx_t j = 1;; ++j) {
        idx_t val1 = j * (3 * j - 1) / 2;      /* k > 0 */
        if (val1 > n) break;
        is_gp[val1] = 1;
        idx_t val2 = j * (3 * j + 1) / 2;      /* k < 0 */
        if (val2 <= n) is_gp[val2] = 1;
    }

    /*--------------------------------------------------------------*/
    /* 3.  Count the representations.                            */
    /*--------------------------------------------------------------*/
    seq_t count = 0;
    /* t ranges from 0 to n-1 (could be 0 because floor(1/3)=0 and 3*2±1 primes) */
    for (idx_t t = 0; t < n; ++t) {
        /* Check if t is in A257121: exists m∈{3t,3t+1,3t+2} with 3m±1 primes */
        bool in_set = false;
        for (idx_t offset = 0; offset <= 2; ++offset) {
            idx_t m = 3 * t + offset;
            idx_t p1 = 3 * m - 1;
            idx_t p2 = 3 * m + 1;
            if (p1 <= maxP && p2 <= maxP && prime[p1] && prime[p2]) {
                in_set = true;
                break;
            }
        }
        if (!in_set) continue;

        idx_t diff = n - t;                    /* diff > 0 because t < n */
        if (diff >= gp_size) continue;         /* safety, though should not happen */
        if (is_gp[diff]) {
            count += (seq_t)1;
        }
    }

    free(prime);
    free(is_gp);
    return count;
}
