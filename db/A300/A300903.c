/*
 * A300903  a(n) is the smallest k such that k^2 - n^2 is a prime power
 *          (A000961), or 0 if no such k exists.
 * https://oeis.org/A300903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 90, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = smallest k  (k > n)  such that  k² – n²  is a prime power
 *  (1 is regarded as a prime power).  If no such k exists, return 0.
 *
 *  Computation:  test k = n+1 … 3⋅n.  For each k check whether
 *  diff = k² – n² is one‑prime‑power by trial division using a
 *  pre‑computed list of primes up to ≈ 3·n.   The list is built once
 *  on first call.
 */

/* -----  helper includes  ----- */
#include <stddef.h>
#include <stdbool.h>

/* -----  primes up to a fixed limit  ----- */
#define PRIME_SIEVE_LIMIT   30000        /* √(8·10⁴²) ≈ 28284, so this works for n≤10 000 */

static unsigned int primes[PRIME_SIEVE_LIMIT + 1];
static size_t prime_cnt = 0;

/*  Build the array of primes once.  */
static void build_primes(void)
{
    if (prime_cnt) return;                       /* already built */

    bool flags[PRIME_SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) flags[i] = true;
    flags[0] = flags[1] = false;

    for (size_t p = 2; p * p <= PRIME_SIEVE_LIMIT; ++p)
        if (flags[p])
            for (size_t q = p * p; q <= PRIME_SIEVE_LIMIT; q += p)
                flags[q] = false;

    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (flags[i]) primes[prime_cnt++] = (unsigned int)i;
}

/*  Return true if x (x ≥ 1) is a prime power (including 1).  */
static bool is_prime_power(seq_t x)
{
    if (x == (seq_t)1) return true;          /* 1 counts as a prime power */

    seq_t remaining = x;
    bool seen = false;

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];

        /* avoid overflow: stop when p*p > remaining
         *   iff remaining / p < p
         */
        if (remaining / p < p) break;

        if (remaining % p == (seq_t)0) {
            if (seen) return false;          /* more than one distinct prime */
            seen = true;
            while (remaining % p == (seq_t)0)
                remaining /= p;
        }
    }

    if (remaining == (seq_t)1) return true;   /* completely factored */
    if (seen) return false;                   /* a second distinct prime */
    return true;                              /* remaining itself is a single prime */
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition (1² – 0² = 1).  */
    if (n == (idx_t)0) return (seq_t)1;

    build_primes();

    /*  Theoretical upper bound: k ≤ 3n.  */
    idx_t max_k = 3 * n;
    seq_t n_sq = (seq_t)n * (seq_t)n;

    for (idx_t k = n + 1; k <= max_k; ++k) {
        seq_t k_sq   = (seq_t)k * (seq_t)k;
        seq_t diff   = k_sq - n_sq;              /* k² – n² ≥ 1 */
        if (is_prime_power(diff)) return (seq_t)k;
    }
    return (seq_t)0;                            /* no suitable k found */
}
