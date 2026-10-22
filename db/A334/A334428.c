/*
 * A334428  Irregular triangle read by rows: row n gives the members of the
 *          smallest nonnegative reduced residue system in the modified
 *          congruence modulo 2*n - 1 by Brändli and Beyne, called mod*(2*n -
 *          1).
 * https://oeis.org/A334428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A334428
 *
 *  Formula (algorithmic description):
 *  Let r be the smallest integer such that
 *      sum_{i=1}^{r-1} φ(2*i-1)/2   <   n   ≤   sum_{i=1}^{r} φ(2*i-1)/2,
 *  with the convention φ(1)=1 and φ(1)/2=1 for r=1.
 *  Then a(n) is the k‑th smallest integer in [1,\,2*r-1) that is coprime to 2*r-1,
 *  where k = n - sum_{i=1}^{r-1} φ(2*i-1)/2.
 *  For r=1, a(1)=0.
 *
 *  The implementation uses a small prime sieve (up to 20000) to compute
 *  φ(m) efficiently and then scans for the needed coprime.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Maximum value of 2*n-1 for n up to 10000 */
#define MAXM 20000

static idx_t primes[2300];
static size_t prime_cnt = 0;
static bool primes_ready = false;

/* ------------------------------------------------------------------ */
/* Simple sieve to fill the primes array (once).                      */
static void init_primes(void)
{
    bool is_comp[MAXM + 1] = {false};
    for (idx_t i = 2; i <= MAXM; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= MAXM) {
                for (idx_t j = i * i; j <= MAXM; j += i)
                    is_comp[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* ------------------------------------------------------------------ */
/* Greatest common divisor for seq_t (works for built‑in and big int).*/
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Euler's totient of m (m ≥ 1).  Uses the pre‑computed primes. */
static seq_t phi_of(idx_t m)
{
    if (m == 1) return (seq_t)1;
    seq_t result = m;
    idx_t temp = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

/* ------------------------------------------------------------------ */
/* Length of row r: 1 for r=1 else φ(2r-1)/2. */
static idx_t row_len(idx_t r)
{
    if (r == 1) return 1;
    idx_t m = 2 * r - 1;
    seq_t phi = phi_of(m);
    return (idx_t)(phi / 2);
}

/* ------------------------------------------------------------------ */
/* Return the k-th smallest integer in [1, m) coprime to m.
 *   m ≥ 3, 1 ≤ k ≤ φ(m)/2.                                    */
static seq_t kth_coprime(idx_t m, idx_t k)
{
    idx_t count = 0;
    for (idx_t v = 1; v < m; ++v) {
        if (gcd_seq_t((seq_t)v, (seq_t)m) == (seq_t)1) {
            ++count;
            if (count == k) return (seq_t)v;
        }
    }
    return (seq_t)0;          /* should not happen for valid input */
}

/* ------------------------------------------------------------------ */
/* Main sequence function: a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (!primes_ready) init_primes();

    if (n == 1) return (seq_t)0;           /* row 1, only element 0 */

    idx_t remaining = n;
    idx_t r = 1;
    while (true) {
        idx_t len = row_len(r);
        if (remaining <= len) break;
        remaining -= len;
        ++r;
    }

    /* r is the row containing the n-th element,
       remaining is the position within that row (1-based). */
    if (r == 1) return (seq_t)0;            /* safety, should not be reached */

    idx_t m = 2 * r - 1;
    idx_t k = remaining;
    return kth_coprime(m, k);
}
