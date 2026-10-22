/*
 * A327930  Product_{d|n, d>1} prime(A003415(d)), where A003415(x) gives the
 *          arithmetic derivative of x.
 * https://oeis.org/A327930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} prime(A003415(d)) where A003415 is the
 *            arithmetic derivative of d, and prime(k) is the k-th prime (OEIS
 *            A000040)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 35, u64 n <= 119, u128 n <= 359,
 *            big any n
 * Helpers:   build_spf, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"   /* provides: seq_t get_prime(idx_t k); */
#include "../utils/build_spf.h"   /* provides: void build_spf(idx_t limit, idx_t **spf_out); */

static idx_t *spf_arr = NULL;
static idx_t spf_limit = 0;

/* Ensure that spf_arr contains the smallest prime factors up to n. */
static void ensure_spf(idx_t n)
{
    if (spf_arr && spf_limit >= n)
        return;
    if (spf_arr)
        free(spf_arr);
    build_spf(n, &spf_arr);
    spf_limit = n;
}

/* Compute the arithmetic derivative of a divisor d (d > 1). */
static idx_t deriv_of_divisor(idx_t d)
{
    seq_t d_seq = (seq_t)d;
    seq_t deriv = 0;
    idx_t temp = d;
    while (temp > 1) {
        idx_t p = spf_arr[temp];
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        deriv += (d_seq / (seq_t)p) * (seq_t)e;
    }
    return (idx_t)deriv;   /* fits in idx_t for the given limits */
}

/* Recursive enumeration of all divisors of n from its prime factorization.
   curr is the current divisor value being built. */
static void rec_divs(int pos, seq_t curr,
                     const idx_t *primes, const int *exps, int cnt,
                     seq_t *product)
{
    if (pos == cnt) {
        if (curr > 1) {                     /* skip divisor 1 */
            idx_t d = (idx_t)curr;
            idx_t deriv = deriv_of_divisor(d);
            seq_t prime_val = get_prime(deriv);
            *product = (*product) * prime_val;
        }
        return;
    }

    seq_t val = curr;
    for (int e = 0; e <= exps[pos]; ++e) {
        rec_divs(pos + 1, val, primes, exps, cnt, product);
        val *= primes[pos];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 by definition */

    ensure_spf(n);

    /* Factor n to obtain its prime factors and exponents. */
    idx_t temp = n;
    idx_t primes[32];
    int exps[32];
    int cnt = 0;
    while (temp > 1) {
        idx_t p = spf_arr[temp];
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        primes[cnt] = p;
        exps[cnt] = e;
        ++cnt;
    }

    seq_t product = (seq_t)1;
    rec_divs(0, (seq_t)1, primes, exps, cnt, &product);
    return product;
}
