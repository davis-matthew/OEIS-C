/*
 * A277170  Numerator of 3F2([3*n, -n, n+1],[2*n+1, n+1/2], 1).
 * https://oeis.org/A277170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 37, i32 n <= 66, i64 n <= 160,
 *            i128 n <= 364, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A277170 – numerator of 3F2([3*n, -n, n+1],[2*n+1, n+1/2], 1)
   Formula:
     a(2k)   = (( (4k)! * (3k)! ) / ((6k)! * k! ))^2
     a(2k+1) = -1/3 * (( (4k+1)! * (3k)! ) / ((6k+1)! * k! ))^2
   The implementation computes the exponent of each prime in the reduced
   fraction and multiplies the primes raised to these exponents.  A simple
   sieve generates primes up to the required bound.  All arithmetic uses
   the seq_t type. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ---------- Helpers ---------- */

/* Return the exponent of prime p in N! using Legendre's formula. */
static int exp_factorial(idx_t N, uint32_t p)
{
    int e = 0;
    idx_t div = p;
    while (div <= N) {
        e += (int)(N / div);
        div *= p;
    }
    return e;
}

/* Fast powering for small integer exponent (e >= 0). */
static seq_t pow_seq(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* ---------- Prime generation ---------- */

static uint32_t *prime_list = NULL;   /* array of primes */
static size_t   prime_count = 0;      /* number of primes */
static idx_t    prime_limit = 0;      /* maximum n sieved up to */

/* Allocate and store all primes up to 'limit' (inclusive). */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* free any previous allocation */
    if (prime_list)
        free(prime_list);

    if (limit < 2) {
        prime_list = NULL;
        prime_count = 0;
        prime_limit = limit;
        return;
    }

    /* Sieve of Eratosthenes up to 'limit'. */
    size_t size = (size_t)(limit + 1);          /* number of candidates 0..limit */
    char *is_prime = (char *)malloc(size);
    if (!is_prime)
        exit(EXIT_FAILURE);

    memset(is_prime, 1, size);
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes to allocate the array. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    /* Allocate the array. */
    if (cnt == 0) {
        prime_list = NULL;
        prime_count = 0;
        prime_limit = limit;
        free(is_prime);
        return;
    }

    prime_list  = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) {
        free(is_prime);
        exit(EXIT_FAILURE);
    }

    /* Fill the array. */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint32_t)i;

    prime_count = cnt;
    prime_limit = limit;
    free(is_prime);
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Determine k and parity. */
    idx_t k     = n >> 1;        /* floor(n/2) */
    int   odd   = n & 1;         /* 1 if n is odd */

    /* Need primes up to 6k (+1 for odd n). */
    idx_t need = (6 * k) + (odd ? 1 : 0);
    ensure_primes_upto(need);

    /* Allocate exponent array. */
    int *exp_arr = (int *)calloc(prime_count, sizeof(int));
    if (!exp_arr)
        exit(EXIT_FAILURE);

    /* Compute exponents of each prime in the reduced fraction. */
    for (size_t pi = 0; pi < prime_count; ++pi)
    {
        uint32_t p = prime_list[pi];
        int e = 0;
        if (odd) {
            e += exp_factorial(4 * k + 1, p);
            e += exp_factorial(3 * k,     p);
            e -= exp_factorial(6 * k + 1, p);
            e -= exp_factorial(k,         p);
        } else {
            e += exp_factorial(4 * k,     p);
            e += exp_factorial(3 * k,     p);
            e -= exp_factorial(6 * k,     p);
            e -= exp_factorial(k,         p);
        }
        e *= 2;                     /* square the ratio */
        if (odd && p == 3)
            e -= 1;                 /* divide by 3 for odd n */
        exp_arr[pi] = e;
    }

    /* Build the integer result from prime exponents. */
    seq_t result = 1;
    for (size_t pi = 0; pi < prime_count; ++pi) {
        int e = exp_arr[pi];
        if (e > 0) {
            seq_t pw = pow_seq((seq_t)prime_list[pi], e);
            result *= pw;
        }
    }

    free(exp_arr);

    /* Apply sign for odd n. */
    if (odd)
        result = -result;

    return result;
}
