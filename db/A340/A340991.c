/*
 * A340991  Triangle T(n,k) whose k-th column is the k-fold self-convolution
 *          of the primes; triangle T(n,k), n>=0, 0<=k<=n, read by rows.
 * https://oeis.org/A340991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r(r+1)/2 <= n < (r+1)(r+2)/2 and T(n,k) =
 *            [x^n]( Σ_{j≥1} prime(j)·x^j )^k. The triangle is built with the
 *            recurrence T(r,0) = 0 (r>0), T(0,0) = 1, T(r,k) = Σ_{j=1..r}
 *            prime(j)·T(r-j,k-1) (k>0)
 * Range:     u8 n <= 23, u16 n <= 70, u32 n <= 219, u64 n <= 759,
 *            u128 n <= 2739, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------------------------------------------------------- */
/*  Pre‑compute a small list of primes.  The sieve limit is   */
/*  chosen to contain at least 200 primes (more than enough for */
/*  the required rows).                                         */
/* ----------------------------------------------------------- */
#define PRIME_SIEVE_LIMIT 2000          /* > 200th prime (1223)   */
#define MAX_PRIMES 350                  /* number of primes below the limit */

static uint32_t prime_list[MAX_PRIMES];
static size_t   prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt > 0) return;           /* already done */

    bool sieve[PRIME_SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t p = 2; p * p <= PRIME_SIEVE_LIMIT; ++p) {
        if (sieve[p]) {
            for (size_t m = p * p; m <= PRIME_SIEVE_LIMIT; m += p)
                sieve[m] = false;
        }
    }

    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT && prime_cnt < MAX_PRIMES; ++i)
        if (sieve[i]) prime_list[prime_cnt++] = (uint32_t)i;
}

/* ----------------------------------------------------------- */
/*  Triangle storage.  triangle[r][k] == T(r,k).  The maximum   */
/*  row needed for n ≤ 10 000 is < 150, so we allow up to 200. */
/* ----------------------------------------------------------- */
#define MAX_ROWS 200

static seq_t *triangle[MAX_ROWS];   /* pointers to allocated rows   */
static size_t triangle_rows_done = 0;

/* ----------------------------------------------------------- */
/*  Build rows up to ‘rows_needed’ (inclusive of row 0).      */
/* ----------------------------------------------------------- */
static void compute_triangle_up_to(size_t rows_needed)
{
    init_primes();

    if (rows_needed <= triangle_rows_done) return;
    if (rows_needed > MAX_ROWS) rows_needed = MAX_ROWS;    /* safety */

    for (size_t r = triangle_rows_done; r < rows_needed; ++r) {
        /* allocate the current row */
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        triangle[r] = row;

        for (size_t k = 0; k <= r; ++k) {
            if (r == 0 && k == 0) {
                row[0] = (seq_t)1;      /* T(0,0)=1 */
            } else if (k == 0) {
                row[0] = (seq_t)0;      /* T(r,0)=0 for r>0 */
            } else {
                seq_t sum = (seq_t)0;
                for (size_t j = 1; j <= r; ++j) {
                    /* Skip if the prime index exceeds what we have */
                    if (j > prime_cnt) break;
                    seq_t pj   = (seq_t)prime_list[j - 1];
                    size_t prev_row = r - j;
                    size_t prev_col = k - 1;
                    if (prev_col <= prev_row) {
                        seq_t prev = triangle[prev_row][prev_col];
                        sum += pj * prev;      /* multiply & add in seq_t */
                    }
                }
                row[k] = sum;
            }
        }
    }
    triangle_rows_done = rows_needed;
}

/* ----------------------------------------------------------- */
/*  Public function required by the build.                     */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    size_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= (size_t)n) ++r;

    size_t col = (size_t)n - r * (r + 1) / 2;

    /* Ensure the triangle is computed up to row r. */
    compute_triangle_up_to(r + 1);

    return triangle[r][col];
}
