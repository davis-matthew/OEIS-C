/*
 * A284763  Numbers n such that A279513(n) is squarefree.
 * https://oeis.org/A284763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A284763 : Numbers n such that the prime‑tower factorisation of n
 *            contains no duplicate prime.
 *
 * The function a(idx_t n) returns the n-th term of this sequence,
 * with a(1)=1, a(2)=2, a(3)=3, a(4)=5, …
 *
 * Implementation notes
 * --------------------
 * * The test `in_sequence` factors the input using a small prime sieve
 *   and walks the prime tower recursively, keeping a list of primes
 *   already seen.  If a prime occurs twice, the factorisation fails
 *   and the number is excluded.
 *
 * * The main algorithm simply scans integers from 1 upwards until
 *   the desired index is reached; this is fast enough for the
 *   required range (n up to about 10 000) and keeps the code
 *   straightforward.  Because the function may be called with
 *   arbitrary indices in any order, no memoisation of the
 *   cumulative list is used.
 *
 * * The implementation uses only the facilities required by the
 *   exercise; no additional external libraries are needed.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PRIME_SIEVE_LIMIT 2000000       /* sufficient for all required values */

static bool composite[PRIME_SIEVE_LIMIT + 1];
static uint32_t primes[150000];         /* pi(2e6) ≈ 148933 */
static size_t primes_n = 0;
static bool primes_ready = false;

/* Build the prime list once. */
static void init_primes(void)
{
    if (primes_ready) return;

    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) composite[i] = false;
    composite[0] = composite[1] = true;
    for (uint32_t i = 2; (uint64_t)i * (uint64_t)i <= PRIME_SIEVE_LIMIT; ++i)
    {
        if (!composite[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                composite[j] = true;
        }
    }
    primes_n = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!composite[i]) primes[primes_n++] = i;
    primes_ready = true;
}

/* Context for remembering primes already seen. */
struct ctx
{
    uint32_t seen[64];   /* enough for all tested values */
    int count;
};

/* Add a prime to the context; return false if it was already present. */
static inline bool ctx_add_prime(struct ctx *c, uint32_t p)
{
    for (int i = 0; i < c->count; ++i)
        if (c->seen[i] == p) return false;   /* duplicate found */
    c->seen[c->count++] = p;
    return true;
}

/* Recursively factorise x following the prime‑tower rule. */
static bool factor_tower(uint64_t x, struct ctx *c)
{
    if (x <= 1) return true;

    for (size_t i = 0; i < primes_n && (uint64_t)primes[i] * (uint64_t)primes[i] <= x; ++i)
    {
        uint32_t p = primes[i];
        if (x % p == 0)
        {
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if (!ctx_add_prime(c, p)) return false;
            if (exp > 1 && !factor_tower((uint64_t)exp, c)) return false;
        }
    }
    if (x > 1) {                                  /* remaining prime factor */
        if (!ctx_add_prime(c, (uint32_t)x)) return false;
    }
    return true;
}

/* True iff n belongs to the sequence. */
static bool in_sequence(uint64_t n)
{
    init_primes();
    struct ctx c = { .count = 0 };
    return factor_tower(n, &c);
}

/* ------------------------------------------------------------------ */
/* Sequence function                                                    */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined for index 0 */

    seq_t val = 1;           /* current integer to test */
    idx_t found = 0;         /* number of terms already found */

    while (found < n) {
        if (in_sequence((uint64_t)val))
            ++found;
        ++val;
    }

    return val - 1;          /* val already overshot by one after loop */
}
