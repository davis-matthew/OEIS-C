/*
 * A397932  Numbers k that are neither squarefree nor powerful such that
 *          rad(k)*omega(k) = k, where rad = A007947 and omega = A001221.
 * https://oeis.org/A397932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k that are neither squarefree nor powerful such that
 *            rad(k)*omega(k) = k. The nth term is returned by a(idx_t n)
 * Range:     u8 n <= 21, u16 n <= 4136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for the smallest‑prime‑factor sieve */
static size_t _spf_limit = 0;
static uint32_t *_spf = NULL;

/* initialise a sieve of size limit (inclusive).  Recalculates the whole array. */
static void _init_spf(size_t limit)
{
    if (limit <= _spf_limit) return;

    uint32_t *new_spf = (uint32_t *)realloc(_spf, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);      /* out of memory */
    _spf = new_spf;
    _spf_limit = limit;

    /* simple Eratosthenes sieve for spf */
    for (size_t i = 0; i <= limit; ++i) _spf[i] = 0;
    _spf[0] = 0; _spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (_spf[i] == 0) {
            _spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (_spf[j] == 0) _spf[j] = (uint32_t)i;
            }
        }
    }
}

/* main function requested by the OEIS tool */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but keep safe */

    /* initial sieve limit – enough for the first 10000 terms.  If it fails,
       the limit is doubled on demand. */
    size_t limit = 2000000;
    _init_spf(limit);

    idx_t found = 0;
    seq_t result = 0;
    for (idx_t cur = 2; ; ++cur) {
        /* extend sieve when needed */
        if ((size_t)cur > limit) {
            limit *= 2;
            _init_spf(limit);
        }

        idx_t x = cur;
        seq_t rad   = 1;          /* product of distinct primes */
        idx_t omega = 0;          /* number of distinct primes */
        bool has_exp1  = false;
        bool has_exp2p = false;

        uint32_t last_prime = 0;
        size_t exp_count = 0;

        while (x > 1) {
            uint32_t p = _spf[x];
            if (p == 0) p = (uint32_t)x;      /* should not happen */
            if (p != last_prime) {
                if (last_prime != 0) {
                    if (exp_count == 1) has_exp1 = true;
                    else if (exp_count >= 2) has_exp2p = true;
                }
                omega++;
                rad = rad * (seq_t)p;          /* multiply once per distinct prime */
                last_prime = p;
                exp_count = 0;
            }
            exp_count++;
            x /= p;
        }
        if (last_prime != 0) {                /* finish the last prime */
            if (exp_count == 1) has_exp1 = true;
            else if (exp_count >= 2) has_exp2p = true;
        }

        /* check the defining condition */
        seq_t cand   = (seq_t)cur;
        seq_t prod   = rad * (seq_t)omega;
        if ((prod == cand) && has_exp1 && has_exp2p) {
            found++;
            if (found == n) {
                result = cand;
                break;
            }
        }
    }
    return result;
}
