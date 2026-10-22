/*
 * A353862  Greatest run-sum of the prime indices of n.
 * https://oeis.org/A353862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the maximum product (prime_index * exponent)
   among all prime factors of n.  For n=1 the result is 0.  The prime
   indices are 1‑based: prime(1)=2, prime(2)=3, prime(3)=5, …      */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum n we expect to handle (given in the specification)   */
#define MAX_N 10000

/* Number of primes ≤ MAX_N (π(10000) = 1229) */ 
#define PRIME_COUNT 1229

static uint64_t primes[PRIME_COUNT];   /* array of primes ≤ MAX_N */
static bool primes_inited = false;     /* flag set once after sieve */

/* Simple sieve to fill primes[ ] with primes ≤ MAX_N.              */
static void init_primes(void)
{
    if (primes_inited)
        return;
    bool composite[MAX_N + 1] = {false};
    size_t pos = 0;
    for (uint64_t i = 2; i <= MAX_N; ++i) {
        if (!composite[i]) {
            primes[pos++] = i;
            if (pos == PRIME_COUNT)
                break;
            for (uint64_t j = i * i; j <= MAX_N; j += i)
                composite[j] = true;
        }
    }
    primes_inited = true;
}

/* Return the 1‑based index of prime p in the global primes array.  */
static idx_t prime_index(uint64_t p)
{
    /* Linear search – π(10000)=1229 so it is fast enough.      */
    for (idx_t i = 0; i < PRIME_COUNT; ++i)
        if (primes[i] == p)
            return i + 1; /* 1‑based */
    return 0; /* should never happen for valid inputs within bounds */
}

seq_t a(idx_t n)
{
    /* Special case: n = 1 has no prime factors.                 */
    if (n <= 1)
        return (seq_t)0;

    init_primes();                 /* ensure primes[ ] is populated */

    seq_t best = 0;                /* best run‑sum found so far       */
    uint64_t m = (uint64_t)n;      /* work on a copy of n             */

    /* Compute floor(sqrt(n)) without floating point.           */
    idx_t s = 0;
    while ((s + 1) * (s + 1) <= n)
        ++s;

    /* Trial divide by all primes ≤ sqrt(n).                    */
    for (idx_t i = 0; i < PRIME_COUNT && primes[i] <= s; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t run = (seq_t)(i + 1) * (seq_t)exp; /* i+1 = index  */
            if (run > best)
                best = run;
        }
    }

    /* If a remaining prime factor > sqrt(n) exists.             */
    if (m > 1) {                     /* m is a prime ≤ MAX_N        */
        idx_t i = prime_index(m);
        seq_t run = (seq_t)i; /* exponent is 1 */
        if (run > best)
            best = run;
    }

    return best;
}
