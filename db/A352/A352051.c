/*
 * A352051  Sum of the 5th powers of the divisor complements of the odd proper
 *          divisors of n.
 * https://oeis.org/A352051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 * Sum_{d|n, d<n, d odd} 1 / d^5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7127,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* If n == 0 the sequence is undefined; return 0 */
    if (n == 0) return (seq_t)0;

    /* Compute odd part of n (i.e., remove all factors of 2) */
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;

    /* Factor the odd part m */
    idx_t primes[64];
    int    exps[64];
    int    pcnt = 0;

    idx_t t = m;
    /* test factor 3 explicitly for speed */
    if (t % 3 == 0) {
        int e = 0;
        while (t % 3 == 0) {
            t /= 3;
            e++;
        }
        primes[pcnt] = 3;
        exps[pcnt]   = e;
        pcnt++;
    }
    /* trial division by odd numbers starting at 5 */
    for (idx_t p = 5; p * p <= t; p += 2) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            pcnt++;
        }
    }
    if (t > 1) {
        primes[pcnt] = t;
        exps[pcnt]   = 1;
        pcnt++;
    }

    /* Generate all divisors of m (all odd divisors of n) */
    seq_t divs[8192];
    int dcnt = 1;
    divs[0] = (seq_t)1;

    for (int i = 0; i < pcnt; i++) {
        idx_t p = primes[i];
        int   e = exps[i];
        int oldcnt = dcnt;
        seq_t pk = (seq_t)1;
        for (int k = 1; k <= e; k++) {
            pk *= (seq_t)p;          /* p^k */
            for (int j = 0; j < oldcnt; j++) {
                divs[dcnt++] = divs[j] * pk;
            }
        }
    }

    /* Sum the terms (n/d)^5 over all odd divisors d < n */
    seq_t sum = (seq_t)0;
    seq_t sn  = (seq_t)n;
    for (int j = 0; j < dcnt; j++) {
        seq_t d = divs[j];
        seq_t q = sn / d;          /* exact division, q = n / d */
        seq_t term = q;
        term *= q;                /* q^2 */
        term *= q;                /* q^3 */
        term *= q;                /* q^4 */
        term *= q;                /* q^5 */
        sum += term;
    }

    /* Exclude the divisor d = n when n is odd */
    if ((n & 1) != 0) {
        /* term with q = 1 corresponds to d = n; subtract 1^5 = 1 */
        sum -= (seq_t)1;
    }

    return sum;
}
