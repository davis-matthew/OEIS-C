/*
 * A328572  Primorial base expansion of n converted into its prime product
 *          form, but with 1 subtracted from all nonzero digits: a(n) =
 *          A003557(A276086(n)).
 * https://oeis.org/A328572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   Primorial base expansion of n, convert digits (minus 1) to
 *            exponents of corresponding primes and take the product. a(n) =
 *            ∏_{i} prime_i ^ ( (n div P_i mod prime_i) - 1 ) where the
 *            exponent is 0 if the digit is 0. (P_i = product of primes
 *            strictly less than prime_i, P_0 = 1)
 * Range:     u8 n <= 27, u16 n <= 147, u32 n <= 1047, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Maximum number of primes we store – sufficient for n up to very large values. */
#define MAX_PRIMES 128

static uint64_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;

/* Returns the smallest prime > start.  Stores primes in the static array. */
static uint64_t next_prime_value(uint64_t start)
{
    uint64_t cand = start + 1;
    while (1) {
        /* Check divisibility by known primes up to sqrt(cand). */
        size_t i;
        for (i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0)
                break;
        }
        if (i < prime_cnt && primes[i] * primes[i] <= cand && cand % primes[i] == 0)
            ++cand; /* composite – try next candidate */
        else {
            /* cand is prime */
            if (prime_cnt < MAX_PRIMES)
                primes[prime_cnt++] = cand;
            return cand;
        }
    }
}

/* Helper: computes base^exp for small exp (exp <= prime-1). */
static seq_t power_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 automatically by the loop below. */
    seq_t product = 1;
    idx_t remaining = n;
    uint64_t prev_prime = 1;   /* Start with a number < the smallest prime. */
    while (remaining > 0) {
        uint64_t prime = next_prime_value(prev_prime);  /* next prime needed for this digit. */
        idx_t digit = remaining % prime;
        idx_t exp = (digit > 0) ? (digit - 1) : 0;
        if (exp > 0) {
            seq_t pows = power_small((seq_t)prime, exp);
            product *= pows;
        }
        remaining /= prime;
        prev_prime = prime;
    }
    return product;
}
