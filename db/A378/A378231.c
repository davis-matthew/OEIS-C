/*
 * A378231  Deficiency of prime-shifted squares: a(n) = 2*A003961(n^2) -
 *          sigma(A003961(n^2)), where A003961 is fully multiplicative
 *          function with a(prime(i)) = prime(i+1).
 * https://oeis.org/A378231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*A003961(n^2) - sigma(A003961(n^2))
 * Range:     i8 n <= 7, i16 n <= 62, i32 n <= 2047, i64 n <= 9403,
 *            i128 n <= 9403, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Prime sieve limit – enough for all next‑prime lookups needed
   for n up to 10000. */
#define LIMIT 20000

static bool is_comp[LIMIT + 1];
static uint32_t primes[3000];
static size_t prime_cnt = 0;
static bool primes_initialized = false;

/* Initialise the prime tables on first use. */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* Sieve of Eratosthenes. */
    for (int i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;
        }
    }

    /* Store primes in the array. */
    for (int i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i])
            primes[prime_cnt++] = (uint32_t)i;
    }

    primes_initialized = true;
}

/* Linear search for the index of a prime in the primes array. */
static int find_prime_index(uint32_t p)
{
    /* p is guaranteed to be prime and <= LIMIT. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (primes[i] == p)
            return (int)i;
    }
    return -1;            /* should never happen for valid input */
}

/* Compute a(n) for the defined sequence. */
seq_t a(idx_t n)
{
    init_primes();

    /* Special case n = 1 – no prime factors. */
    if (n == (idx_t)1)
        return (seq_t)1;            /* 2*1 - 1 = 1 */

    /* Factor n, collecting primes and exponents. */
    uint64_t tmp = (uint64_t)n;
    uint32_t pf[16];      /* maximum distinct primes for n <= 10000 is 5, margin added */
    int   exp[16];
    size_t pf_cnt = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            int e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            pf[pf_cnt] = primes[i];
            exp[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {          /* remaining factor is a prime > sqrt(original n). */
        pf[pf_cnt] = (uint32_t)tmp;
        exp[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* Evaluate A003961(n^2) and its sigma. */
    seq_t f_total = (seq_t)1;     /* product of (next prime)^e2 */
    seq_t sigma   = (seq_t)1;     /* product of sums 1 + p + ... + p^e2 */

    for (size_t i = 0; i < pf_cnt; ++i) {
        int e2 = 2 * exp[i];          /* exponent in n^2 */
        int idx = find_prime_index(pf[i]);
        uint32_t nextp = primes[idx + 1];   /* guaranteed to exist */

        seq_t pow_next = (seq_t)1;
        seq_t sum_next = (seq_t)1;

        for (int j = 0; j < e2; ++j) {
            pow_next *= (seq_t)nextp;
            sum_next += pow_next;
        }

        f_total *= pow_next;
        sigma   *= sum_next;
    }

    /* a(n) = 2 * f_total - sigma */
    return (seq_t)((seq_t)2 * f_total - sigma);
}
