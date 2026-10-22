/*
 * A395148  Square array A(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where A(n,k) = (2*n)! * [x^(2*n)] 1/(1 - x^2)^(k/2).
 * https://oeis.org/A395148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of OEIS A395148, read by antidiagonals of
 *            the array A(n,k) defined by A(n,k) = (2*n)! * [x^(2*n)] 1/(1 -
 *            x^2)^(k/2). For n>0, A(n,0)=0; otherwise the value is an integer
 *            that can be computed from the prime factorisation of (2n)! / n!
 *            * ∏_{t=0}^{n-1}(k+2t) / 2^n (n=row, k=col)
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 42, u64 n <= 88, u128 n <= 207,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

seq_t a(idx_t N)
{
    /* -----------------   initialise small prime table   ----------------- */
    static bool primes_initialized = false;
    static int primes[200];
    static size_t prime_count = 0;

    if (!primes_initialized) {
        bool is_prime[501];
        for (size_t i = 0; i < 501; ++i) is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (int p = 2; p <= 500; ++p) {
            if (is_prime[p]) {
                primes[prime_count++] = p;
                if ((long long)p * p <= 500) {
                    for (int q = p * p; q <= 500; q += p)
                        is_prime[q] = false;
                }
            }
        }
        primes_initialized = true;
    }

    /* -----------------   map global index to (row,col)   ----------------- */
    idx_t d = (idx_t)((sqrtl((long double)8.0L * N + 1.0L) - 1.0L) / 2.0L);
    /* correct possible rounding error of the floating‑point estimate */
    while (((__int128)(d + 1) * (d + 2)) / 2 <= N) ++d;
    while (((__int128)d * (d + 1)) / 2 > N) --d;

    idx_t offset = N - d * (d + 1) / 2;   /* position inside the diagonal */
    idx_t row    = offset;                /* n in the array definition   */
    idx_t col    = d - offset;            /* k in the array definition   */

    /* A(n,0) = 0 for all n>0 */
    if (col == 0 && row > 0) return 0;

    /* -----------------   prime‑exponent array   ----------------- */
    int exp[200];
    memset(exp, 0, sizeof(exp));

    /* factor numbers 1 .. 2*row  (numerator of (2n)! ) */
    for (idx_t i = 1; i <= 2 * row; ++i) {
        idx_t x = i;
        for (size_t pi = 0; pi < prime_count &&
                           (idx_t)primes[pi] * (idx_t)primes[pi] <= x; ++pi) {
            int p = primes[pi];
            int cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            if (cnt) exp[pi] += cnt;
        }
        if (x > 1) { /* remaining prime factor */
            for (size_t pi = 0; pi < prime_count; ++pi)
                if (primes[pi] == (int)x) { exp[pi] += 1; break; }
        }
    }

    /* subtract numbers 1 .. row  (denominator of n! ) */
    for (idx_t i = 1; i <= row; ++i) {
        idx_t x = i;
        for (size_t pi = 0; pi < prime_count &&
                           (idx_t)primes[pi] * (idx_t)primes[pi] <= x; ++pi) {
            int p = primes[pi];
            int cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            if (cnt) exp[pi] -= cnt;
        }
        if (x > 1) {
            for (size_t pi = 0; pi < prime_count; ++pi)
                if (primes[pi] == (int)x) { exp[pi] -= 1; break; }
        }
    }

    /* multiply the product ∏_{t=0}^{row-1}(col+2t)   */
    for (idx_t t = 0; t < row; ++t) {
        idx_t x = col + 2 * t;
        for (size_t pi = 0; pi < prime_count &&
                           (idx_t)primes[pi] * (idx_t)primes[pi] <= x; ++pi) {
            int p = primes[pi];
            int cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            if (cnt) exp[pi] += cnt;
        }
        if (x > 1) {
            for (size_t pi = 0; pi < prime_count; ++pi)
                if (primes[pi] == (int)x) { exp[pi] += 1; break; }
        }
    }

    /* divide by 2^row  –  subtract row from the exponent of 2 */
    for (size_t pi = 0; pi < prime_count; ++pi)
        if (primes[pi] == 2) { exp[pi] -= (int)row; break; }

    /* -----------------   reconstruct the integer   ----------------- */
    seq_t res = 1;
    for (size_t pi = 0; pi < prime_count; ++pi) {
        int e = exp[pi];
        if (e > 0) {
            seq_t base = (seq_t)primes[pi];
            for (int k = 0; k < e; ++k) res *= base;
        }
        /* negative exponents cannot occur for valid arguments */
    }
    return res;
}
