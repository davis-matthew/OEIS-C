/*
 * A296419  Triangle T(i,j) read by rows: Number of plane bipolar orientations
 *          with i+1 vertices and j+1 faces.
 * https://oeis.org/A296419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(i,j) = 2*(i+j-2)!*(i+j-1)!*(i+j)! /
 *            ((i-1)!*i!*(i+1)!*(j-1)!*j!*(j+1)!) (read triangle by rows)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 43, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* small primes up to 300 */
static int primes[300];
static int prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void) {
    if (primes_inited) return;
    bool composite[301] = {false};
    for (int p = 2; p <= 300; ++p) {
        if (!composite[p]) {
            primes[prime_cnt++] = p;
            for (int q = p * p; q <= 300; q += p)
                composite[q] = true;
        }
    }
    primes_inited = true;
}

/* exponent of prime p in n! (Legendre's formula) */
static int exp_factorial(idx_t n, int p) {
    int e = 0;
    idx_t div = p;
    while (div <= n) {
        e += n / div;
        if (div > n / p) break;   /* prevent overflow of div*p */
        div *= p;
    }
    return e;
}

seq_t a(idx_t n) {
    /* locate row i and column j in the triangle (1‑based) */
    init_primes();
    idx_t sum = 1;      /* cumulative count of entries up to current row */
    idx_t i = 1;
    while (sum < n) {
        ++i;
        sum += i;
    }
    idx_t sum_prev = sum - i;
    idx_t j = n - sum_prev;   /* 1‑based column */

    /* compute prime exponents for T(i,j) */
    int exp[64];  /* prime_cnt <= 62 for primes <= 300 */
    for (int k = 0; k < prime_cnt; ++k) exp[k] = 0;

    for (int k = 0; k < prime_cnt; ++k) {
        int p = primes[k];
        /* numerator */
        exp[k] += exp_factorial(i + j - 2, p);
        exp[k] += exp_factorial(i + j - 1, p);
        exp[k] += exp_factorial(i + j,     p);
        if (p == 2) exp[k] += 1;          /* constant factor 2 */
        /* denominator */
        exp[k] -= exp_factorial(i - 1, p);
        exp[k] -= exp_factorial(i,     p);
        exp[k] -= exp_factorial(i + 1, p);
        exp[k] -= exp_factorial(j - 1, p);
        exp[k] -= exp_factorial(j,     p);
        exp[k] -= exp_factorial(j + 1, p);
    }

    /* assemble result from prime powers */
    seq_t result = 1;
    for (int k = 0; k < prime_cnt; ++k) {
        int p = primes[k];
        int e = exp[k];
        while (e--) {
            result *= (seq_t)p;
        }
    }
    return result;
}
