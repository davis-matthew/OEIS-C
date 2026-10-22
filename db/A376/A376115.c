/*
 * A376115  Least common differences in the arithmetic progressions
 *          corresponding to A376109.
 * https://oeis.org/A376115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 623, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A376115: Least common differences in the arithmetic progressions
 * corresponding to A376109.
 *
 * a(n) = least d ≥ 1 such that for all i with 0 ≤ i < k,
 *        A001222(n – i·d) = A001222(n), where
 *        k = A376109(n) = maximum length of an arithmetic progression
 *        ending at n in which every term has the same big‑Omega value as n.
 * The function computes k by scanning all positive differences d.
 * Among all d that attain k, the smallest d is returned.
 *
 * Implementation details:
 * - Pre‑computes smallest prime factors and big‑Omega values up to the
 *   largest n requested so far (expanded on demand).
 * - Uses only integer arithmetic compatible with C11 / C++17.
 * - Works for arbitrary‑precision seq_t as returned by this function.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global sieve state */
static idx_t   _sieve_cap = 0;      /* largest n covered so far */
static uint32_t * _spf    = NULL;   /* smallest prime factor array, 0 means prime */
static uint8_t  * _omega  = NULL;   /* big‑Omega values */

/* Ensure that _spf and _omega cover numbers 0..n (inclusive). */
static void _ensure_sieve(idx_t n)
{
    if (n <= _sieve_cap) return;               /* already sufficient */

    /* free old tables */
    if (_spf) free(_spf);
    if (_omega) free(_omega);

    /* allocate new tables */
    _spf   = (uint32_t*)calloc((size_t)n + 1, sizeof(uint32_t));
    _omega = (uint8_t*)calloc((size_t)n + 1, sizeof(uint8_t));
    if (!_spf || !_omega) { free(_spf); free(_omega); exit(1); }

    /* smallest prime factor sieve */
    _spf[0] = 0;
    _spf[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (_spf[i] == 0) {          /* i is prime */
            _spf[i] = (uint32_t)i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (_spf[j] == 0) _spf[j] = (uint32_t)i;
            }
        }
    }

    /* compute big‑Omega as we go */
    _omega[1] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        uint32_t p = _spf[i];
        _omega[i] = _omega[i / p] + 1;
    }

    _sieve_cap = n;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but guard */

    _ensure_sieve(n);             /* have big‑Omega up to n */

    uint8_t w = _omega[n];        /* big‑Omega of n */
    idx_t   best_len = 1;         /* at least the number n itself */
    seq_t   best_d   = (seq_t)1;  /* smallest difference seen so far */

    for (idx_t d = 1; d < n; ++d) {
        idx_t term = n;
        idx_t len  = 1;           /* includes n */

        /* walk backwards while within bounds and Omega matches */
        while (term > d) {
            term -= d;
            if (_omega[term] != w) break;
            ++len;
        }

        if (len > best_len || (len == best_len && d < (idx_t)best_d)) {
            best_len = len;
            best_d   = (seq_t)d;
        }
    }

    return best_d;
}
