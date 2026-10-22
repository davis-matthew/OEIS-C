/*
 * A330550  Gasket based on primes: take array read by diagonals from
 *          Gilbreath conjecture, omitting the initial row of primes (A036261)
 *          and replace all nonzero entries with 1's.
 * https://oeis.org/A330550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the (i,j)-entry of the Gilbreath array (constructed
 *            from the primes by successive absolute differences) is non‑zero,
 *            where (i,j) is obtained by reading the array along antidiagonals
 *            starting at (0,0); 0 otherwise
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*  Primes generation (only small number needed)                       */
/* ------------------------------------------------------------------ */
#define MAX_PRIME_COUNT 2000
static uint64_t primes[MAX_PRIME_COUNT];
static size_t prime_cnt = 0;

static void ensure_primes(size_t need) {
    if (prime_cnt >= need) return;
    if (prime_cnt == 0) { primes[prime_cnt++] = 2; }
    uint64_t cand = primes[prime_cnt - 1] + 1;
    while (prime_cnt < need) {
        bool ok = true;
        uint64_t limit = (uint64_t)(sqrt((double)cand) + 0.5);
        for (size_t k = 0; k < prime_cnt && primes[k] <= limit; ++k) {
            if (cand % primes[k] == 0) { ok = false; break; }
        }
        if (ok) primes[prime_cnt++] = cand;
        ++cand;
    }
}

/* ------------------------------------------------------------------ */
/*  Antidiagonal coordinates                                            */
/* ------------------------------------------------------------------ */
static void antidiag_coords(idx_t n, idx_t *ri, idx_t *cj) {
    idx_t k = n - 1;                          /* 0‑based index   */
    double  val = sqrt((double)(8 * k + 1));
    idx_t   s   = (idx_t)((val - 1.0) / 2.0 + 1e-12); /* floor          */
    idx_t   prev = s * (s + 1) / 2;
    idx_t   d = k - prev;
    *ri = s - d;
    *cj = d;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */
#define MAX_BUF 400     /* Safe upper bound for rows/columns */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Offset 1; behaviour before 1 is irrelevant */

    idx_t i, j;
    antidiag_coords(n, &i, &j);    /* (row, col) of n‑th element */

    /* Primes required for the deepest column reached */
    size_t needPrimes = j + i + 2;
    ensure_primes(needPrimes);

    /* Buffers for successive rows of finite differences */
    seq_t row1[MAX_BUF], row2[MAX_BUF];
    seq_t *prev = row1, *curr = row2;

    /* Build the 0th row (differences of consecutive primes) */
    idx_t row_len = j + i + 1;
    for (idx_t k = 0; k < row_len; ++k) {
        prev[k] = (seq_t)(primes[k + 1] - primes[k]); /* primes are increasing */
    }

    /* Generate rows 1..i */
    for (idx_t level = 1; level <= i; ++level) {
        idx_t new_len = row_len - level;
        for (idx_t k = 0; k < new_len; ++k) {
            seq_t a1 = prev[k];
            seq_t a2 = prev[k + 1];
            curr[k] = (a1 > a2) ? (a1 - a2) : (a2 - a1);   /* absolute difference */
        }
        /* swap buffers for next level */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t val = prev[j];
    return (val > (seq_t)0) ? (seq_t)1 : (seq_t)0;
}
