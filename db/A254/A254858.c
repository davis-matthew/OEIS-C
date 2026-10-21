/*
 * A254858  Iterated partial sums of prime numbers, square array read by
 *          diagonals.
 * https://oeis.org/A254858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 160, u32 n <= 609, u64 n <= 2241,
 *            u128 n <= 7259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254858: Iterated partial sums of prime numbers, read by diagonals
   Formula: a(n) = T[row][col] where (row,col) are the anti‑diagonal coordinates
   with row + col = n and rows defined by iterated partial sums of primes.
   Implementation computes row = d - k, col = k with
     d = max{ t | t(t+1)/2 <= n }          (triangular index)
     k = n - d(d+1)/2
   and evaluates the required element by iterating the partial sums up to
   the needed row and column.  Only primes up to the maximal column index are
   required, which is trivial for the supported range. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper to generate primes up to a required index */
#define MAX_PRIMES 200000
static uint64_t _prime_arr[MAX_PRIMES];
static size_t _prime_cnt = 0;
static uint64_t _prime_next_candidate = 2;

static void _ensure_primes(size_t needed_index)
{
    if (_prime_cnt > needed_index) return;
    if (_prime_next_candidate < 2) _prime_next_candidate = 2;
    while (_prime_cnt <= needed_index) {
        bool is_p = true;
        for (size_t i = 0; i < _prime_cnt &&
             (uint64_t)_prime_arr[i] * (uint64_t)_prime_arr[i] <= _prime_next_candidate;
             ++i) {
            if (_prime_next_candidate % _prime_arr[i] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            if (_prime_cnt >= MAX_PRIMES) break;
            _prime_arr[_prime_cnt++] = _prime_next_candidate;
        }
        ++_prime_next_candidate;
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Find triangular index d: largest d with d(d+1)/2 <= n */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) d++;

    idx_t tri = d * (d + 1) / 2;
    idx_t k   = n - tri;          /* offset within diagonal */
    idx_t i   = d - k;            /* row index        */
    idx_t j   = k;                /* column index     */

    /* Need primes up to column j */
    _ensure_primes((size_t)j);

    /* Allocate small arrays for row computation; j is tiny (~sqrt(2n)) */
    size_t len = (size_t)(j + 1);
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev || !curr) {
        /* Allocation failed; return 0 (should not happen in tests) */
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0;
    }

    /* Row 0: primes */
    for (size_t col = 0; col < len; ++col) {
        prev[col] = (seq_t)_prime_arr[col];
    }

    /* Iterate rows 1..i */
    for (idx_t r = 1; r <= i; ++r) {
        if (len == 0) break;
        curr[0] = prev[0];           /* first element stays the same */
        for (size_t col = 1; col < len; ++col) {
            curr[col] = curr[col - 1] + prev[col];
        }
        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = (len == 0) ? (seq_t)0 : prev[j];

    free(prev);
    free(curr);
    return result;
}
