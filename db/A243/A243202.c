/*
 * A243202  Coefficients of a particular decomposition of N^N in terms of
 *          binomial coefficients.
 * https://oeis.org/A243202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = c(N,j) where N = floor((sqrt(8·n+1)-1)/2) j = n -
 *            N·(N+1)/2 c(N,j) = 0 if N==0 or j==0 N! if j==N N^(N-j) * (j/N)
 *            * j! if 0<j<N The function evaluates this expression directly,
 *            caching factorials
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 78, u64 n <= 171,
 *            u128 n <= 435, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *fact = NULL;
static size_t fact_cap = 0;

static void ensure_factorial(size_t n)
{
    if (n + 1 <= fact_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!new_fact) exit(1); /* allocation failure */

    if (!fact) {
        new_fact[0] = (seq_t)1;
        for (size_t i = 1; i < new_cap; ++i)
            new_fact[i] = new_fact[i-1] * (seq_t)i;
    } else {
        for (size_t i = fact_cap; i < new_cap; ++i)
            new_fact[i] = new_fact[i-1] * (seq_t)i;
    }
    fact = new_fact;
    fact_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Find N such that N·(N+1)/2 ≤ n < (N+1)·(N+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t N = low;
    idx_t T_N = N * (N + 1) / 2;
    idx_t j = n - T_N;

    if (N == 0 || j == 0) return (seq_t)0;

    ensure_factorial((size_t)N);

    if ((idx_t)j == N) return fact[N];

    /* Compute N^(N-j-1) */
    seq_t pow = (seq_t)1;
    idx_t exp = N - j - 1;
    for (idx_t k = 0; k < exp; ++k)
        pow *= (seq_t)N;

    seq_t result = pow * (seq_t)j * fact[j];
    return result;
}
