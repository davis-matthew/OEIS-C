/*
 * A268145  Twin prime pairs concatenated to their average in decimal
 *          representation (with the lesser twin prepended and the greater
 *          twin appended).
 * https://oeis.org/A268145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(p, p+1, p+2) where (p,p+2) is the nth twin‑prime
 *            pair (p prime)
 * Range:     u8 none, u16 n <= 2, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   digits, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  Utility functions supplied by the build system  */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x);  */
#include "../utils/digits.h"    /* int digits(idx_t x);     */
#include "../utils/pow10.h"     /* seq_t pow10(idx_t exp); */

/*  Static cache of lesser primes of twin prime pairs  */
static idx_t *twin_less = NULL;   /* array of p such that (p, p+2) are twin primes */
static size_t twin_cnt   = 0;      /* number of cached twin pairs */
static size_t twin_cap   = 0;      /* allocated capacity of twin_less */
static idx_t next_check  = 3;      /* next number to test for twin primes */

/*  Ensure that at least n twin primes are cached (n >= 1)  */
static void
ensure_twin_primes(idx_t n)
{
    if (twin_cnt >= n)
        return;

    /* Grow the cache if necessary */
    if (twin_cap == 0) {
        twin_cap = 64;
        twin_less = (idx_t *)malloc(twin_cap * sizeof(idx_t));
        if (!twin_less) abort();
    }

    while (twin_cnt < n) {
        /* Find the next twin prime after next_check */
        idx_t p = next_check;
        if (p < 3) p = 3;
        if ((p & 1) == 0) ++p;          /* ensure p is odd */

        for (;;) {
            if (is_prime(p) && is_prime(p + 2)) {
                /* Store the found twin prime pair */
                if (twin_cnt == twin_cap) {
                    twin_cap *= 2;
                    idx_t *tmp = (idx_t *)realloc(twin_less, twin_cap * sizeof(idx_t));
                    if (!tmp) abort();
                    twin_less = tmp;
                }
                twin_less[twin_cnt++] = p;
                /* Next candidate starts at the following odd number to enable overlapping pairs */
                next_check = p + 2;
                break;
            }
            p += 2; /* skip even numbers */
        }
    }
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    /* Offset is 1; for n == 0 return 0 just in case */
    if (n == 0)
        return (seq_t)0;

    ensure_twin_primes(n);

    idx_t p   = twin_less[n - 1];   /* lesser twin prime  */
    idx_t q   = p + 2;              /* greater twin prime */
    idx_t avg = p + 1;              /* average (non‑prime)   */

    /* Number of decimal digits of avg and q */
    int d_avg = digits(avg);
    int d_q   = digits(q);

    /* Compute powers of 10 for concatenation */
    seq_t pow_pq = pow10((idx_t)(d_avg + d_q));
    seq_t pow_q  = pow10((idx_t)d_q);

    /* Concatenate: p || avg || q  */
    seq_t term = (seq_t)p * pow_pq;
    term += (seq_t)avg * pow_q;
    term += (seq_t)q;

    return term;
}
