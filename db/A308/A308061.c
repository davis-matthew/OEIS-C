/*
 * A308061  Number of integer-sided triangles with perimeter n whose side
 *          lengths are squarefree.
 * https://oeis.org/A308061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered integer‑sided triangles with
 *            perimeter n whose side lengths are squarefree. Computed by
 *            enumerating all triples a ≤ b ≤ c, a + b + c = n, a + b > c, and
 *            counting only squarefree sides
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Local integer square root (floor) without using external utilities. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* Allocate and initialise the square‑free flag array. */
    uint8_t *sf = (uint8_t *)malloc((size_t)n + 1);
    if (!sf)
        return (seq_t)0;            /* allocation failure, return 0 */
    for (idx_t i = 1; i <= n; ++i)
        sf[i] = 1;                  /* 1 means squarefree initially */

    /* Sieve primes up to sqrt(n) and use them to mark non‑squarefree numbers. */
    idx_t sqrt_n = isqrt_idx(n);
    uint8_t *is_prime = (uint8_t *)malloc((size_t)sqrt_n + 1);
    if (!is_prime) {
        free(sf);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= sqrt_n; ++i)
        is_prime[i] = 1;
    if (sqrt_n >= 0) is_prime[0] = 0;
    if (sqrt_n >= 1) is_prime[1] = 0;

    for (idx_t i = 2; i * i <= sqrt_n; ++i)
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= sqrt_n; j += i)
                is_prime[j] = 0;
        }

    for (idx_t p = 2; p <= sqrt_n; ++p)
        if (is_prime[p]) {
            idx_t p2 = p * p;
            for (idx_t k = p2; k <= n; k += p2)
                sf[k] = 0;          /* not squarefree */
        }

    /* Count unordered triangles with squarefree sides. */
    seq_t cnt = 0;
    for (idx_t a = 1; a <= n / 3; ++a) {
        if (!sf[a])
            continue;
        idx_t max_b = (n - a) / 2;
        for (idx_t b = a; b <= max_b; ++b) {
            if (!sf[b])
                continue;
            idx_t c = n - a - b;
            if (c < b)          /* maintain order a ≤ b ≤ c */
                continue;
            if (a + b <= c)     /* triangle inequality must hold */
                continue;
            if (!sf[c])
                continue;
            ++cnt;
        }
    }

    free(sf);
    free(is_prime);
    return cnt;
}
