/*
 * A368674  Sum of the squarefree numbers less than n that do not divide n.
 * https://oeis.org/A368674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=1..n} k·μ(k)²·(⌈n/k⌉ – ⌊n/k⌋)
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum reduces to the sum of all square‑free k < n that do not divide n. */

#include <stdbool.h>
#include <stddef.h>

#if !defined(PRIME_LIMIT)
#define PRIME_LIMIT 1000   /* enough for n up to about 1 000 000 (√n ≤ 1000) */
#endif

static bool primes_ready = false;
static size_t primes_cnt  = 0;
static int   primes[PRIME_LIMIT];

/* Sieve once to fill the primes array */
static void init_primes(void)
{
    if (primes_ready) return;

    bool sieve[PRIME_LIMIT];
    for (size_t i = 0; i < PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (int i = 2; i * i < PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j < PRIME_LIMIT; j += i) sieve[j] = false;
        }
    }

    for (int i = 2; i < PRIME_LIMIT; ++i) {
        if (sieve[i]) primes[primes_cnt++] = i;
    }

    primes_ready = true;
}

/* Test if k is square‑free using the precomputed primes */
static bool is_squarefree(idx_t k)
{
    if (k < 2) return true;
    for (size_t i = 0; i < primes_cnt; ++i) {
        int p  = primes[i];
        int p2 = p * p;
        if ((idx_t)p2 > k) break;
        if (k % p2 == 0) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();                 /* primes ready once */
    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k < n; ++k) {
        if (n % k == 0) continue;        /* k divides n → contribution zero */
        if (!is_squarefree(k)) continue; /* μ(k)² = 0 for non–square‑free */
        sum += (seq_t)k;
    }

    return sum;
}
