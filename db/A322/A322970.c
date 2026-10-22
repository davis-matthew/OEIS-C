/*
 * A322970  Coefficient triangle of polynomials recursively defined by P(n,x)
 *          = (n+1)*(n+1)! + x*Sum_{k=1..n} k^2*n!/(n+1-k)!*P(n-k,x) with
 *          P(0,x) = 1.
 * https://oeis.org/A322970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  A322970:  Coefficient triangle of the recursively defined polynomials
 *
 *  The sequence is obtained by enumerating the rows of the triangle
 *  A(n,k) = (n+1)! * (n+1+k)! / ((k+1)! * (2k+1)! * (n-k)!)
 *  where row n has n+1 coefficients for k = n,n-1,…,0.
 *
 *  a(0) = 1
 *  a(n) = A(r, r - offset)   where
 *      r          = minimal integer with r(r+1)/2 > n
 *      offset     = n - r(r-1)/2   (offset inside the row, 0‑based)
 *
 *  The implementation uses prime‑exponent factorisation of the
 *  factorials so that no intermediate overflow can occur for
 *  any n that fits in the native seq_t type.
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define MAX_PRIME 40000

static int  primes[MAX_PRIME];
static int  prime_cnt = 0;
static bool primes_inited = false;

/* generate all primes up to MAX_PRIME */
static void init_primes(void)
{
    if (primes_inited) return;
    static bool comp[MAX_PRIME + 1];
    memset(comp, 0, sizeof(comp));

    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = i;
            if ((long long)i * i <= MAX_PRIME)
                for (int j = i * i; j <= MAX_PRIME; j += i)
                    comp[j] = true;
        }
    }
    primes_inited = true;
}

/* exponent of prime p in n! (Legendre's formula) */
static idx_t exp_fact(idx_t n, idx_t p)
{
    idx_t res = 0;
    while (n) {
        n /= p;
        res += n;
    }
    return res;
}

/* main function required by the test harness */
seq_t a(idx_t n)
{
    /* find the row r that contains index n */
    idx_t r = 0;
    idx_t cum = 1;                     /* sum of 1..(r+1) */
    while (n >= cum) {
        ++r;
        cum = (r + 1) * (r + 2) / 2;
    }

    idx_t prev_cum = r * (r + 1) / 2;
    idx_t offset   = n - prev_cum;     /* 0‑based inside the row */
    idx_t k        = r - offset;       /* coefficient index in OEIS formula */

    init_primes();

    /* highest factorial argument needed */
    idx_t limit = r + 1 + k;          /* = 2r+1 at most for this n */
    if (limit > (idx_t)prime_cnt) limit = prime_cnt;  /* safe guard */

    seq_t result = 1;

    for (int pi = 0; pi < prime_cnt; ++pi) {
        unsigned int p = primes[pi];
        if (p > limit) break;

        idx_t e = exp_fact(r + 1, p)
                + exp_fact(r + 1 + k, p)
                - exp_fact(k + 1, p)
                - exp_fact(2 * k + 1, p)
                - exp_fact(r - k, p);

        while (e--) {
            result *= (seq_t)p;
        }
    }
    return result;
}
