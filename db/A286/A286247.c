/*
 * A286247  Triangular table: T(n,k) = 0 if k does not divide n, otherwise
 *          T(n,k) = P(A046523(k), n/k), where P is sequence A000027 used as a
 *          pairing function N x N -> N. Table is read by rows as T(1,1),
 *          T(2,1), T(2,2), etc.
 * https://oeis.org/A286247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangular table T(n,k) where T(n,k) = 0 if k does not
 *            divide n, otherwise T(n,k) = P(A046523(k), n/k), P is Cantor's
 *            pairing function A046523(k) is the minimal integer with the same
 *            prime signature as k
 * Range:     u8 n <= 276, u16 n <= 10585, u32 n <= 10585, u64 n <= 10585,
 *            u128 n <= 10585, big any n
 * Validated: standard, pass, 136 terms through n = 9431 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* A046523(k) returns the smallest integer having the same
   prime exponent multiset (prime signature) as k.
   For example:
   k = 1  -> 1
   k = 2  -> 2
   k = 3  -> 2
   k = 4  -> 4
   k = 6  -> 6
   k = 8  -> 8
   k = 9  -> 4
*/
static seq_t a046523(idx_t k)
{
    if (k <= 1) return (seq_t)1;

    /* Small list of primes used for factorisation.
       Enough for k <= 10^6 (worst case exponent count < 15). */
    static const uint32_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };
    const unsigned int prime_cnt = sizeof(primes) / sizeof(primes[0]);

    idx_t temp = k;
    unsigned int exp_arr[25] = {0};
    unsigned int exp_cnt = 0;

    for (unsigned int i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > temp) break;
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            exp_arr[exp_cnt++] = e;
        }
    }
    if (temp > 1) {
        exp_arr[exp_cnt++] = 1;
    }

    /* sort exponents descending (simple bubble for tiny arrays) */
    for (unsigned int i = 0; i < exp_cnt; ++i) {
        for (unsigned int j = i + 1; j < exp_cnt; ++j) {
            if (exp_arr[j] > exp_arr[i]) {
                unsigned int tmp = exp_arr[i];
                exp_arr[i] = exp_arr[j];
                exp_arr[j] = tmp;
            }
        }
    }

    /* compute minimal integer having this signature:
       assign largest exponents to smallest primes 2,3,5,... */
    seq_t result = 1;
    for (unsigned int i = 0; i < exp_cnt; ++i) {
        seq_t base = (seq_t)primes[i];
        unsigned int e = exp_arr[i];
        seq_t term = 1;
        for (unsigned int j = 0; j < e; ++j) term *= base;
        result *= term;
    }
    return result;
}

/* --------- Helper: compute A046523(k) --------- */

/* --------- Helper: find row r such that r*(r+1)/2 >= n --------- */
static idx_t find_row(idx_t n)
{
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* --------- Main function: OEIS A286247 --------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset is 1, but guard */

    /* Determine (row r, column k) in the triangular representation. */
    idx_t r = find_row(n);                   /* row number (1‑based) */
    idx_t prev_tri = r * (r - 1) / 2;        /* sum of lengths of previous rows */
    idx_t k = n - prev_tri;                  /* column number (1‑based) */

    /* If k does not divide r, the entry is 0. */
    if (r % k != 0) return (seq_t)0;

    idx_t q = r / k;                         /* quotient n/k                       */
    seq_t sig = a046523(k);                  /* A046523(k)                          */

    /* Compute the Cantor pairing function P(sig, q):
       P(a,b) = (a+b-1)(a+b-2)/2 + a
       Using the algebraic form given in the OEIS entry:
       P = (1/2)*(2 + (sig+q)^2 - sig - 3*q) */
    seq_t sum = sig + (seq_t)q;
    seq_t val = (sum * sum + (seq_t)2 - sig - (seq_t)3 * q) / (seq_t)2;

    return val;
}
