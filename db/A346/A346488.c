/*
 * A346488  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), for all i, j >= 1, where f(n) = 0 if mu(n) = -1,
 *          and f(n) = n for all other numbers (with mu = Möbius mu, A008683).
 * https://oeis.org/A346488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1: if μ(n) = -1, a(n) = a(first index where μ
 *            = -1) (which equals that index); else a(n) = n - (# of indices ≤
 *            n with μ = -1) + 1
 * Range:     u8 n <= 364, u16 n <= 9526, u32 n <= 9526, u64 n <= 9526,
 *            u128 n <= 9526, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state */
static idx_t _max_n   = 0;
static int8_t * _mu    = NULL;   /* Möbius values: -1,0,1 */
static idx_t * _cnt    = NULL;   /* cumulative count of μ = -1 up to index */
static idx_t   _first_neg1 = 0;   /* first index with μ = -1 */

/* Build tables up to N (inclusive). */
static void _compute_tables(idx_t N)
{
    /* resize buffers if needed */
    if (_max_n < N) {
        _mu  = (int8_t *)realloc(_mu,  (N + 1) * sizeof(int8_t));
        _cnt = (idx_t *)realloc(_cnt, (N + 1) * sizeof(idx_t));
        _max_n = N;
    }

    /* initialise */
    for (idx_t i = 0; i <= N; ++i) {
        _mu[i]  = 0;
        _cnt[i] = 0;
    }
    _mu[1] = 1;

    /* linear sieve for Möbius */
    int   *primes = (int *)malloc((N + 1) * sizeof(int));
    size_t pr_cnt = 0;
    idx_t  *lp = (idx_t *)malloc((N + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= N; ++i) lp[i] = 0;

    for (idx_t i = 2; i <= N; ++i) {
        if (lp[i] == 0) {                /* i is prime */
            lp[i] = i;
            primes[pr_cnt++] = (int)i;
            _mu[i] = -1;
        }
        for (size_t j = 0; j < pr_cnt; ++j) {
            idx_t p   = (idx_t)primes[j];
            idx_t m   = i * p;
            if (m > N) break;
            lp[m] = p;
            if (_mu[i] == 0) {
                _mu[m] = 0;
            } else if (p == (idx_t)primes[0] && (i % (idx_t)primes[0] == 0)) {
                /* handled above to set zero */
            }
            if (lp[i] == p) {
                _mu[m] = 0;
            } else {
                _mu[m] = -_mu[i];
            }
        }
    }
    free(primes);
    free(lp);

    /* cumulative counts of μ = -1 and first negative */
    idx_t cum = 0;
    for (idx_t i = 1; i <= N; ++i) {
        if (_mu[i] == -1) {
            ++cum;
            if (_first_neg1 == 0) _first_neg1 = i;
        }
        _cnt[i] = cum;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    if (n > _max_n) _compute_tables(n);

    if (n == 1) return (seq_t)1;

    if (_mu[n] == -1) {
        /* all μ = -1 indices share the label of the first occurrence */
        return (seq_t)_first_neg1;
    }

    /* a(n) = n - count_mu_minus1(n) + 1, provided n ≥ first_neg1 */
    idx_t count_minus1 = _cnt[n];
    return (seq_t)(n - count_minus1 + 1);
}
