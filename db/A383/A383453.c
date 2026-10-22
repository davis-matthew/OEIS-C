/*
 * A383453  The Geode Bi-Tri infinite rectangular array, read by upward
 *          antidiagonals.
 * https://oeis.org/A383453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the infinite rectangular array
 *            A383453 along upward antidiagonals. The entry at coordinates
 *            (m2,m3) is ( 2*m2 + 3*m3 + 3 )!
 *            -------------------------------------------- ( 2*m2 + 2*m3 + 3 )
 *            * ( m2 + m3 + 1 ) * ( m2 + 2*m3 + 2 )! * m2! * m3! The mapping
 *            from index n to (...
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int primes[100];
static int prime_cnt = 0;
static bool primes_initialized = false;
static void init_primes(void) {
    if (primes_initialized) return;
    const int LIMIT = 423;             /* max value needed for A */
    bool is_prime[LIMIT + 1];
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }
    for (int i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt++] = i;
        }
    }
    primes_initialized = true;
}

/* Add / subtract prime exponents of n!   */
static void update_fact_exp(idx_t n, int sign, int *exp) {
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if (p > n) break;
        int e = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            e += (int)t;
        }
        exp[i] += sign * e;
    }
}

/* Factorize an integer <= 40000 and update exponents */
static void factor_int(idx_t v, int sign, int *exp) {
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        if ((idx_t)p * (idx_t)p > v) break;
        while (v % p == 0) {
            exp[i] += sign;
            v /= p;
        }
    }
    if (v > 1) {          /* v itself is prime */
        /* find its index */
        for (int i = 0; i < prime_cnt; ++i) {
            if (primes[i] == (int)v) {
                exp[i] += sign;
                return;
            }
        }
        /* The prime might exceed precomputed limit (unlikely here) */
        /* Append it to the list if necessary */
        /* For safety, ignore as the value is very small */
    }
}

seq_t a(idx_t n) {
    init_primes();

    /* find diagonal d */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    idx_t prev_total = d * (d + 1) / 2;
    idx_t offset = n - prev_total;
    idx_t m2 = d - offset;
    idx_t m3 = offset;

    /* parameters of the formula */
    idx_t A = 2 * m2 + 3 * m3 + 3;
    idx_t B = 2 * m2 + 2 * m3 + 3;
    idx_t C = m2 + m3 + 1;
    idx_t D = m2 + 2 * m3 + 2;

    /* exponent array for primes */
    int exp[prime_cnt];
    for (int i = 0; i < prime_cnt; ++i) exp[i] = 0;

    /* add factorial(A) exponents */
    update_fact_exp(A, +1, exp);

    /* subtract factorial(D), m2! and m3! exponents */
    update_fact_exp(D, -1, exp);
    update_fact_exp(m2, -1, exp);
    update_fact_exp(m3, -1, exp);

    /* subtract primes from B and C */
    factor_int(B, -1, exp);
    factor_int(C, -1, exp);

    /* reconstruct the result from prime powers */
    seq_t result = 1;
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primes[i];
        for (int k = 0; k < exp[i]; ++k) {
            result = result * (seq_t)p;
        }
    }
    return result;
}
