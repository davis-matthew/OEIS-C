/*
 * A294243  Sum of the larger parts of the partitions of 2n into two parts
 *          with smaller part nonsquarefree.
 * https://oeis.org/A294243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..n} (2*n - i) * (1 - μ(i)²)
 * Range:     u8 n <= 23, u16 n <= 337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where μ(i) is the Möbius function. The implementation
   uses a linear sieve to compute μ(i) for i ≤ n, then
   evaluates the closed‑form expression  
   a(n)= (3·n² - n)/2 - 2·n·Q(n) + S(n) 
   with Q(n)=#squarefree≤n and S(n)=Σ_{squarefree≤n}i.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate arrays for the linear sieve.  */
    int8_t *mu = (int8_t*)malloc((n + 1) * sizeof(int8_t));
    bool   *is_comp = (bool*)calloc((n + 1), sizeof(bool));
    if (!mu || !is_comp) {
        free(mu);
        free(is_comp);
        return 0;   /* Allocation failure – return 0.  */
    }

    /* Linear sieve for μ.  */
    int *primes = (int*)malloc((n + 1) * sizeof(int));
    size_t prime_cnt = 0;
    mu[1] = 1;          /* μ(1)=1 */

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = (int)i;
            mu[i] = -1;
        }

        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = (idx_t)primes[j];
            idx_t ip = i * p;
            if (ip > n) break;
            is_comp[ip] = true;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    /* Compute Q(n) and S(n).  */
    seq_t Q = 0;          /* number of squarefree ≤ n   */
    seq_t S = 0;          /* sum of squarefree ≤ n      */
    for (idx_t i = 1; i <= n; ++i) {
        if (mu[i] != 0) {          /* i is squarefree */
            ++Q;
            S += (seq_t)i;
        }
    }

    /* a(n) = (3*n² - n)/2 - 2*n*Q + S.  */
    seq_t nn = (seq_t)n;
    seq_t part1 = ((seq_t)3 * nn * nn - nn) / (seq_t)2;
    seq_t part2 = (seq_t)2 * nn * Q;
    seq_t result = part1 - part2 + S;

    /* Clean up.  */
    free(mu);
    free(is_comp);
    free(primes);

    return result;
}
