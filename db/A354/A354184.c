/*
 * A354184  a(1) = a(2) = 1, a(n) = (A007947(31*a(n-1)) +
 *          A007947(31*a(n-2)))/31 for n >= 3, i.e., 31*a(n) is the largest
 *          squarefree divisor of 31*a(n-1) plus the largest squarefree
 *          divisor of 31*a(n-2).
 * https://oeis.org/A354184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 85, u32 n <= 2075, u64 n <= 2075,
 *            u128 n <= 2075, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354184
   a(1) = a(2) = 1
   for n ≥ 3:
       a(n) = ( A007947(31*a(n-1)) + A007947(31*a(n-2)) ) / 31
   (A007947(x) is the largest squarefree divisor of x.)
   The sequence becomes periodic with period 207 for n ≥ 6.  */

/* ------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
/* ------------------------------------------------------- */
/* -------------------------------------------------------
   Prime sieve up to a fixed limit
   ------------------------------------------------------- */
#define PRIME_LIMIT 20000
static uint32_t primes[PRIME_LIMIT];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_cnt != 0) return;                 /* already built */
    static unsigned char sieve[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[primes_cnt++] = (uint32_t)i;
}

/* -------------------------------------------------------
   Largest squarefree divisor of x (i.e., product of distinct prime factors)
   ------------------------------------------------------- */
static seq_t rad(seq_t x)
{
    seq_t res = 1;
    seq_t tmp = x;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        seq_t pp = (seq_t)p * p;
        if (pp > tmp) break;
        if (tmp % p == 0) {
            res *= p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) res *= tmp;      /* remaining prime factor > sqrt(original) */
    return res;
}

/* -------------------------------------------------------
   Cache of pre‑computed sequence values
   ------------------------------------------------------- */
#define MAX_N 212                          /* 6 + 207  (period starts at 6) */
static seq_t a_seq[MAX_N + 1];   /* 1‑based indexing, a_seq[0] unused */
static bool seq_computed = false;

static void compute_sequence(void)
{
    if (seq_computed) return;
    init_primes();

    a_seq[1] = 1;
    a_seq[2] = 1;

    for (size_t i = 3; i <= MAX_N; ++i) {
        seq_t rad1 = rad(a_seq[i - 1]);
        seq_t t1   = (rad1 % 31 == 0) ? rad1 / 31 : rad1;
        seq_t rad2 = rad(a_seq[i - 2]);
        seq_t t2   = (rad2 % 31 == 0) ? rad2 / 31 : rad2;
        a_seq[i] = t1 + t2;
    }
    seq_computed = true;
}

/* -------------------------------------------------------
   External function required by the build
   ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n == 0, harmless */

    compute_sequence();

    if (n <= (idx_t)MAX_N) return a_seq[(size_t)n];

    /* Periodicity: for n ≥ 6, a(n) = a( n_mod ) where
       n_mod = 6 + ( (n-6) % 207 )  */
    if (n <= 5) return a_seq[(size_t)n];

    size_t period = 207;
    size_t offset = 6;
    size_t idx = offset + ((size_t)(n - offset) % period);
    return a_seq[idx];
}
