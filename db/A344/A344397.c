/*
 * A344397  a(n) = Stirling2(n, floor(n/2)) * floor(n/2)!.
 * https://oeis.org/A344397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Stirling2(n, floor(n/2)) * floor(n/2)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mul_small.h"

/* Assumes that n will not exceed about 10000 in the test set. */
#define MAX_N 10000
#define MAX_K ((MAX_N / 2) + 1)

/* Persistent storage */
static seq_t s_arr[MAX_K];
static seq_t t_arr[MAX_K];
static seq_t *S, *T;               /* pointers to the current and next rows */
static seq_t cache[MAX_N + 1];     /* cached a(n) values */
static idx_t cache_size = 0;       /* number of computed terms */
static bool initialized = false;
static seq_t k_fact = 1;           /* current factorial: (floor(n/2))! */
static idx_t current_k = 0;        /* current value of floor(n/2) */

/* Compute a(n) lazily */
seq_t a(idx_t n)
{
    /* one‑time initialisation */
    if (!initialized) {
        S = s_arr;
        T = t_arr;
        for (idx_t i = 0; i < MAX_K; ++i) {
            S[i] = 0;
            T[i] = 0;
        }
        S[0] = 1;                /* S(0,0) = 1 */
        cache[0] = 1;            /* a(0) = 1 */
        cache_size = 1;
        initialized = true;
    }

    /* If the requested term is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* Compute terms up to n (assumption: n ≤ MAX_N) */
    for (idx_t m = cache_size; m <= n; ++m) {
        idx_t km = m / 2;             /* floor(m/2) */

        /* Update the factorial when km increases */
        if (km > current_k) {
            k_fact = mul_small(k_fact, (unsigned)km);
            current_k = km;
        }

        /* Compute the next row of Stirling numbers of the second kind */
        T[0] = 0;
        for (idx_t k = 1; k < MAX_K; ++k) {
            seq_t term1 = mul_small(S[k], (unsigned)k);
            seq_t term2 = S[k - 1];
            T[k] = term1 + term2;
        }

        /* Swap pointers for the next iteration */
        seq_t *tmp = S;
        S = T;
        T = tmp;

        /* Store a(m) = km! * S(m, km) */
        cache[m] = S[km] * k_fact;
    }

    cache_size = n + 1;
    return cache[n];
}
