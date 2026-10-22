/*
 * A344847  Sum of the prime numbers in, but not on the border of, an n X n
 *          square array whose elements are the numbers from 1..n^2, listed in
 *          increasing order by rows.
 * https://oeis.org/A344847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of prime numbers in the interior of an n×n array *
 *            that contains the numbers 1..n² filled row‑wise. For n ≤ 2 the
 *            interior * is empty, so a(n)=0
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- required includes ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ---------- static sieve state ---------- */
static uint8_t *prime_sieve   = NULL; /* bitset: 1 → composite, 0 → prime */
static idx_t   sieve_limit   = 0;     /* highest number already sieved (inclusive) */

/* ---------- helpers for bit operations ---------- */
static inline bool _is_composite(idx_t n)
{
    if (n < 2) return true;                     /* 0 and 1 are not prime */
    return ((prime_sieve[n >> 3] >> (n & 7)) & 1) != 0;
}

/* ---------- sieve up to 'limit' (inclusive) ---------- */
static void _sieve_up_to(idx_t limit)
{
    if (limit <= sieve_limit) return;           /* already computed */

    /* free any previous sieve and allocate a fresh bitset */
    if (prime_sieve) free(prime_sieve);
    size_t bytes = (limit >> 3) + 1;             /* ceil((limit+1)/8) */
    prime_sieve = (uint8_t*)malloc(bytes);      /* explicit cast for C++ */
    if (!prime_sieve) abort();
    memset(prime_sieve, 0, bytes);

    /* simple Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (_is_composite(p)) continue;
        for (idx_t x = p * p; x <= limit; x += p) {
            prime_sieve[x >> 3] |= 1 << (x & 7);
        }
    }
    sieve_limit = limit;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;                /* no interior cells */

    idx_t n2 = n * n;
    _sieve_up_to(n2);                           /* ensure primes up to n² */

    seq_t sum = 0;
    for (idx_t i = 2; i <= n - 1; ++i) {        /* interior rows */
        idx_t base = (i - 1) * n;               /* first element of row i */
        for (idx_t j = 2; j <= n - 1; ++j) {    /* interior columns */
            idx_t val = base + j;               /* value at (i, j) */
            if (!_is_composite(val)) sum += (seq_t)val;
        }
    }
    return sum;
}
