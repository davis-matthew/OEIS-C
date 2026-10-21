/*
 * A285603  a(0) = 1; for n>1, a(n) is the denominator of b(n) =
 *          Product_{i=1..n} (prime(i)^2 + 1)/(prime(i)^2 - 1).
 * https://oeis.org/A285603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285603: a(n) = denominator of
 *   Π_{i=1..n} (prime(i)^2 + 1)/(prime(i)^2 - 1)
 *
 * The routine builds prime tables up to LIMIT (≈ 200 000),
 * then for each requested n recomputes the prime exponent
 * differences of the numerator and denominator.  The final
 * denominator is obtained by multiplying all primes whose
 * net exponent is negative.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* maximum prime value needed (sqrt(104729^2+1) < 106 000) */
#define LIMIT 200000

/* prime tables and bookkeeping */
static uint32_t primes[LIMIT];
static size_t prime_count = 0;
static uint32_t spf[LIMIT + 1];      /* smallest prime factor (spf[n] = n for primes) */
static int       prime_index[LIMIT + 1];  /* index of prime in primes array, -1 otherwise */

static int *exp_arr = NULL;          /* array of exponent differences */
static size_t exp_arr_size = 0;
static int sieve_done = 0;

/* ------------------------------------------------------------ */
/* Initial prime sieve: fill primes[], spf[] and prime_index[] */
/* ------------------------------------------------------------ */
static void init_sieve(void)
{
    /* initial setup */
    for (size_t i = 0; i <= LIMIT; ++i) {
        spf[i] = (uint32_t)i;
        prime_index[i] = -1;
    }
    prime_index[0] = prime_index[1] = -1;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {           /* i is prime */
            primes[prime_count++] = (uint32_t)i;
            prime_index[i] = (int)prime_count - 1;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
                }
            }
        }
    }
    /* allocate exponent array */
    exp_arr = (int *)malloc(prime_count * sizeof(int));
    exp_arr_size = prime_count;
    sieve_done = 1;
}

/* ------------------------------------------------------------ */
/* factorization helper: update exp_arr with factors of val   */
/* sign = +1 for numerator, -1 for denominator                */
/* ------------------------------------------------------------ */
static void factor_and_update(uint64_t val, int sign, int *exp)
{
    while (val > 1) {
        uint32_t factor;
        if (val <= (uint64_t)LIMIT) {
            factor = spf[(size_t)val];
        } else {
            /* trial division using the prime list
               (all factors are ≤ sqrt(val) ≤ LIMIT) */
            factor = 0;
            for (size_t pi = 0; pi < prime_count &&
                 (uint64_t)primes[pi] * primes[pi] <= val; ++pi) {
                if (val % primes[pi] == 0) {
                    factor = primes[pi];
                    break;
                }
            }
            if (factor == 0) factor = (uint32_t)val;  /* prime larger than LIMIT */
        }
        uint32_t count = 0;
        while (val % factor == 0) {
            val /= factor;
            ++count;
        }
        /* ignore primes that are not in our table (they are > LIMIT)
           these can only appear in numerators and never affect the
           denominator, so we skip them. */
        if (factor <= LIMIT) {
            int idx = prime_index[factor];
            exp[idx] += sign * (int)count;
        }
    }
}

/* ------------------------------------------------------------ */
/* Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    if (!sieve_done) init_sieve();

    /* reset exponent differences */
    for (size_t i = 0; i < prime_count; ++i) exp_arr[i] = 0;

    /* accumulate exponents for each prime factor up to n */
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = primes[i - 1];
        uint64_t num = (uint64_t)p * (uint64_t)p + 1ULL;   /* p^2 + 1 */

        factor_and_update(num, +1, exp_arr);                /* numerator */
        factor_and_update((uint64_t)p - 1ULL, -1, exp_arr);  /* p - 1   */
        factor_and_update((uint64_t)p + 1ULL, -1, exp_arr);  /* p + 1   */
    }

    /* build the denominator from negative exponents */
    seq_t denom = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i) {
        int e = exp_arr[i];
        if (e < 0) {
            seq_t factor = seq_pow((seq_t)primes[i], (idx_t)(-e));
            denom = seq_mul(denom, factor);
        }
    }
    return denom;
}
