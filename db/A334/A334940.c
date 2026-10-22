/*
 * A334940  Partial sums of A230595.
 * https://oeis.org/A334940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*∑_{p prime ≤ √n} π(⌊n/p⌋) – π(⌊√n⌋)²
 * Range:     u8 n <= 416, u16 n <= 9669, u32 n <= 9669, u64 n <= 9669,
 *            u128 n <= 9669, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (π denotes the prime‑counting function.) */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n. */
static idx_t int_sqrt_u64(idx_t n)
{
    if (n < 2) return n;              /* 0 or 1 */
    idx_t left  = 1;
    idx_t right = n >> 1;             /* > √n for n ≥ 2 */
    idx_t ans   = 1;
    while (left <= right) {
        idx_t mid = (left + right) >> 1;
        /* mid ≤ n / mid   ⇔   mid*mid ≤ n   – avoids overflow */
        if (mid <= n / mid) {
            ans = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ans;
}

/*             a(n) = 2∑_{p prime ≤ √n} π(⌊n/p⌋) – π(⌊√n⌋)²                   */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* safety for n==0 (offset is 1) */

    /* ---------- 1. Sieve of Eratosthenes up to n ---------- */
    bool *isPrime = (bool*)calloc(n + 1, sizeof(bool));
    if (!isPrime) return 0;                  /* allocation failure */

    for (idx_t i = 0; i <= n; ++i) isPrime[i] = true;
    if (n >= 0) isPrime[0] = false;
    if (n >= 1) isPrime[1] = false;

    idx_t sqrt_n = int_sqrt_u64(n);
    for (idx_t p = 2; p <= sqrt_n; ++p) {
        if (isPrime[p]) {
            /* mark multiples of p */
            for (idx_t j = p * p; j <= n; j += p)
                isPrime[j] = false;
        }
    }

    /* ---------- 2. Prefix array of prime counts π(x) ---------- */
    size_t *pi = (size_t*)malloc((n + 1) * sizeof(size_t));
    if (!pi) { free(isPrime); return 0; }

    pi[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        pi[i] = pi[i - 1] + (isPrime[i] ? 1 : 0);

    /* ---------- 3. Compute the formula ---------- */
    seq_t result = 0;

    for (idx_t p = 2; p <= sqrt_n; ++p) {
        if (isPrime[p]) {
            idx_t t = n / p;                   /* floor(n/p) */
            result += (seq_t)2 * (seq_t)pi[t];
        }
    }

    size_t psq = pi[sqrt_n];
    result -= (seq_t)psq * (seq_t)psq;

    free(isPrime);
    free(pi);
    return result;
}
