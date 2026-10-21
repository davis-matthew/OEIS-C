/*
 * A265204  Sum of phi(i) over squarefree numbers i <= n.
 * https://oeis.org/A265204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{1 <= i <= n, i squarefree} phi(i) (OEIS A265204)
 * Range:     u8 n <= 34, u16 n <= 553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t _max_n = 0;
static uint64_t * _phi = NULL;
static unsigned char * _squarefree = NULL;   /* 1 = true, 0 = false */
static unsigned char * _isComposite = NULL;
static uint64_t * _primes = NULL;
static size_t _prime_cnt = 0;

/* Build or extend the auxiliary tables up to N.  */
static void _ensure_sieve(idx_t N)
{
    if (N <= _max_n) return;

    /* free old memory */
    free(_phi);
    free(_squarefree);
    free(_isComposite);
    free(_primes);

    /* allocate new memory */
    _phi = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    _squarefree = (unsigned char *)malloc((N + 1) * sizeof(unsigned char));
    _isComposite = (unsigned char *)malloc((N + 1) * sizeof(unsigned char));
    _primes = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    if (!_phi || !_squarefree || !_isComposite || !_primes) abort();

    /* initialise arrays */
    for (idx_t i = 0; i <= N; ++i) {
        _squarefree[i] = 1;
        _isComposite[i] = 0;
    }
    _squarefree[0] = 0;
    _isComposite[0] = _isComposite[1] = 1;
    _phi[0] = 0;
    _phi[1] = 1;
    _prime_cnt = 0;

    /* linear sieve to compute φ and primes */
    for (idx_t i = 2; i <= N; ++i) {
        if (_isComposite[i] == 0) {
            _primes[_prime_cnt++] = i;
            _phi[i] = i - 1;
        }
        for (size_t j = 0; j < _prime_cnt; ++j) {
            uint64_t p = _primes[j];
            uint64_t ip = p * i;
            if (ip > (uint64_t)N) break;
            _isComposite[ip] = 1;
            if (i % p == 0) {
                _phi[ip] = _phi[i] * p;
                break;
            } else {
                _phi[ip] = _phi[i] * (p - 1);
            }
        }
    }

    /* mark non‑squarefree numbers (multiples of a square) */
    for (size_t j = 0; j < _prime_cnt; ++j) {
        uint64_t p = _primes[j];
        uint64_t sq = p * p;
        if (sq > (uint64_t)N) break;
        for (uint64_t k = sq; k <= (uint64_t)N; k += sq) {
            _squarefree[(idx_t)k] = 0;
        }
    }

    _max_n = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, but safe */
    _ensure_sieve(n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (_squarefree[i]) {
            sum += (seq_t)_phi[i];
        }
    }
    return sum;
}
