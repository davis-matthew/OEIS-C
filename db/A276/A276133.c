/*
 * A276133  Exponent of highest power of 2 dividing the product of the
 *          composite numbers between the n-th prime and the (n+1)-st prime.
 * https://oeis.org/A276133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007814(A061214(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the exponent of 2 in the product of all composite
   integers between the n‑th prime and the (n+1)‑st prime. */

#include <stdint.h>
#include <stdlib.h>

static idx_t sieve_limit = 0;        /* current upper bound of sieve          */
static uint32_t *primes = NULL;      /* list of primes                        */
static size_t prime_count = 0;       /* number of primes found                */
static seq_t *pref = NULL;           /* prefix sums of v2(1..i)               */

/* Simple sieve of Eratosthenes that also builds the prefix sum array of
   v2(k) for k = 1 .. limit.  The arrays are reallocated each time this
   function is called.                                            */
static void extend_sieve(idx_t limit)
{
    if (limit < 2) limit = 2;   /* at least handle 2 */

    /* Allocate and zero the composite flag array. */
    size_t sz = (size_t)limit + 1;
    char *comp = (char *)calloc(sz, sizeof(char));
    if (!comp) abort();

    /* Estimate number of primes to allocate the primes array. */
    size_t primes_cap = (size_t)(limit / 10) + 100;
    uint32_t *new_primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!new_primes) abort();

    /* Sieve */
    size_t pc = 0;
    for (size_t i = 2; i <= limit; ++i)
    {
        if (!comp[i]) {
            if (pc >= primes_cap) {
                primes_cap *= 2;
                new_primes = (uint32_t *)realloc(new_primes, primes_cap * sizeof(uint32_t));
                if (!new_primes) abort();
            }
            new_primes[pc++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    comp[j] = 1;
            }
        }
    }

    /* Allocate prefix sum array */
    seq_t *new_pref = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_pref) abort();

    new_pref[0] = (seq_t)0;
    for (size_t i = 1; i <= limit; ++i) {
        unsigned long long t = (unsigned long long)i;
        /* __builtin_ctzll is defined for i>0 */
        int tz = __builtin_ctzll(t);
        new_pref[i] = new_pref[i-1] + (seq_t)tz;
    }

    /* Replace global data, freeing old memory. */
    free(primes);
    free(pref);
    free(comp);

    primes = new_primes;
    prime_count = pc;
    pref = new_pref;
    sieve_limit = limit;
}

/* Ensure we have at least n+1 primes computed.  The sieve limit is
   increased if necessary.                                                */
static void ensure_primes(idx_t n)
{
    /* n is 1‑based index, we need primes[ n ] at least. */
    while (prime_count <= (size_t)(n + 1)) {
        idx_t new_limit = sieve_limit * 2 + 100;
        if (new_limit <= sieve_limit) new_limit = sieve_limit + 100;
        extend_sieve(new_limit);
    }
}

/* Main function required by the problem statement.                         */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n = 0, but return 0 */

    ensure_primes(n);

    /* Retrieve the n-th and (n+1)-st primes (1‑based). */
    idx_t p_n   = (idx_t)primes[n-1];
    idx_t p_np1 = (idx_t)primes[n];

    /* Sum of v2 from p_n+1 to p_np1-1 inclusive.  This equals
       pref[p_np1-1] - pref[p_n].  When p_np1 == p_n + 1 the
       sum is zero automatically.                                 */
    idx_t high = p_np1 > 0 ? p_np1 - 1 : 0;
    size_t hi = (size_t)high;
    size_t lo = (size_t)p_n;          /* pref array uses 0‑based index */

    return pref[hi] - pref[lo];
}
