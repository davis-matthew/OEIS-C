/*
 * A366842  Number of integer partitions of n whose odd parts have a common
 *          divisor > 1.
 * https://oeis.org/A366842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #partitions of n whose odd parts have a common divisor >
 *            1
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef idx_t idx_t;
typedef seq_t seq_t;

/*-------------------------------*/
/*  Cached tables                 */
/*-------------------------------*/
static idx_t    max_n  = 0;
static seq_t *  p      = NULL;      /* unrestricted partitions   */
static seq_t *  p_odd  = NULL;      /* partitions into odd parts */
static int  *   mu     = NULL;      /* Möbius function            */

/*-------------------------------*/
/*  Initialise tables up to n    */
/*-------------------------------*/
static void init_tables(idx_t n)
{
    if (n <= max_n) return;                /* already initialised */

    /* Allocate new arrays */
    seq_t *new_p     = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *new_p_odd = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    int  *new_mu     = (int*)malloc((n + 1) * sizeof(int));

    /* Zero initialise */
    memset(new_p,     0, (n + 1) * sizeof(seq_t));
    memset(new_p_odd, 0, (n + 1) * sizeof(seq_t));
    memset(new_mu,    0, (n + 1) * sizeof(int));

    /* Base cases */
    new_p[0]     = 1;
    new_p_odd[0] = 1;
    new_mu[1]    = 1;                      /* μ(1) = 1 */

    /* ----------------------------- */
    /*  Unrestricted partitions     */
    /* ----------------------------- */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t s = part; s <= n; ++s) {
            new_p[s] += new_p[s - part];
        }
    }

    /* ----------------------------- */
    /*  Partitions into odd parts   */
    /* ----------------------------- */
    for (idx_t part = 1; part <= n; part += 2) {
        for (idx_t s = part; s <= n; ++s) {
            new_p_odd[s] += new_p_odd[s - part];
        }
    }

    /* ----------------------------- */
    /*  Möbius function (linear sieve) */
    /* ----------------------------- */
    int *is_comp = (int*)calloc(n + 1, sizeof(int));
    int *primes  = (int*)malloc((n + 1) * sizeof(int));
    size_t pc = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            primes[pc++] = (int)i;
            new_mu[i] = -1; /* prime → μ = –1 */
        }
        for (size_t j = 0; j < pc; ++j) {
            idx_t pval = (idx_t)primes[j];
            idx_t ip   = i * pval;
            if (ip > n) break;
            is_comp[ip] = 1;
            if (i % pval == 0) {
                new_mu[ip] = 0;
                break;
            } else {
                new_mu[ip] = -new_mu[i];
            }
        }
    }

    free(is_comp);
    free(primes);

    /* Free old tables */
    free(p);
    free(p_odd);
    free(mu);

    /* Commit new tables */
    p      = new_p;
    p_odd  = new_p_odd;
    mu     = new_mu;
    max_n  = n;
}

/*-------------------------------*/
/*   Sequence function a(n)      */
/*-------------------------------*/
seq_t a(idx_t n)
{
    init_tables(n);

    if (n == 0) return 0;

    /* number of partitions into even parts only */
    seq_t p_even_n = ((n & 1ULL) ? 0 : p[n / 2]);

    /* partitions having at least one odd part */
    seq_t total_odd = p[n] - p_even_n;

    /* partitions whose odd parts have gcd = 1  (g1)   */
    seq_t g1 = 0;
    for (idx_t d = 1; d <= n; d += 2) {          /* odd d ≥ 1                 */
        seq_t f_d = 0;
        idx_t max_o = n / d;                     /* maximum o with n - d*o ≥ 0 */
        idx_t start_o = (n & 1ULL) ? 1 : 0;      /* parity of o = parity of n */

        for (idx_t o = start_o; o <= max_o; o += 2) {
            idx_t r = n - d * o;                /* r is even by construction */
            seq_t term = p_odd[o] * p[r / 2];
            f_d += term;
        }
        seq_t f_prime = f_d - p_even_n;         /* remove partitions with no odd parts */
        g1 += (seq_t)mu[d] * f_prime;          /* Möbius inversion */
    }

    /* finally  a(n) = partitions with odd parts  –  partitions with gcd = 1 */
    seq_t a_val = total_odd;
    if (a_val < g1) a_val = 0;
    else            a_val -= g1;

    return a_val;
}
