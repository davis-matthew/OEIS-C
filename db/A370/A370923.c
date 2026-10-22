/*
 * A370923  Rectangular array read by antidiagonals: row n shows the numbers m
 *          >=2 such that the maximum number of consecutive 0's in (e(1),
 *          e(2), ..., e(k)) is n-1, where p(1)^e(1) * p(2)^e(2) * ... *
 *          p(k)^e(k) is the prime factorization of m.
 * https://oeis.org/A370923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A370923 by enumerating integers and placing them
   in rows based on the maximum consecutive zeros in their prime
   exponent sequences, then reading the rows by antidiagonals. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64() */
#include "../utils/next_prime.h"  /* next_prime() */

typedef struct {
    size_t len;          /* number of elements stored            */
    size_t cap;          /* allocated capacity                   */
    seq_t *data;         /* pointer to stored values              */
} Row;

/* ---------- global state ------------------------------------------ */
static Row *rows      = NULL;        /* array of rows, indexed from 1 */
static size_t rows_cap = 0;          /* current capacity of rows array */
static idx_t   next_num = 2;         /* next integer to be processed   */
static idx_t   current_dmax = 0;     /* largest diagonal precomputed   */

/* prime list (dynamic array of uint64_t) */
static uint64_t *prime_list = NULL;
static size_t    prime_len  = 0;
static size_t    prime_cap  = 0;

/* ------------------------------------------------------------------- */
static void init_prime_list(void)
{
    if (prime_list != NULL) return;
    prime_cap = 16;
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    prime_list[0] = 2;
    prime_list[1] = 3;
    prime_len = 2;
}

static void expand_prime_list_to(uint64_t up_to)
{
    init_prime_list();
    while (prime_len == 0 || prime_list[prime_len - 1] < up_to) {
        uint64_t last = prime_len ? prime_list[prime_len - 1] : 1;
        uint64_t next = last < 2 ? 2 : next_prime(last);
        if (prime_len == prime_cap) {
            prime_cap <<= 1;
            prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        }
        prime_list[prime_len++] = next;
    }
}

static size_t index_of_prime(uint64_t p)
{
    /* binary search in prime_list, returns 1‑based index */
    size_t lo = 0, hi = prime_len - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return mid + 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0; /* should never happen if prime_list is correct */
}

/* ------------------------------------------------------------------- */
static uint16_t row_of_number(uint64_t m)
{
    /* factor m */
    uint64_t pf[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pf, ex, &cnt);
    if (cnt == 0) return 1;           /* m==1 never occurs in the sequence */

    /* ensure all prime factors are in prime_list and get their indices */
    size_t idxs[64];
    for (size_t i = 0; i < cnt; ++i) {
        expand_prime_list_to(pf[i]);
        idxs[i] = index_of_prime(pf[i]);  /* 1‑based */
    }

    /* compute maximum run of consecutive missing primes */
    uint16_t max_run = 0;
    if (idxs[0] > 1) max_run = (uint16_t)(idxs[0] - 1);
    for (size_t i = 1; i < cnt; ++i) {
        uint16_t run = (uint16_t)(idxs[i] - idxs[i - 1] - 1);
        if (run > max_run) max_run = run;
    }
    return (uint16_t)(max_run + 1);
}

/* ------------------------------------------------------------------- */
static void init_rows(void)
{
    if (rows != NULL) return;
    rows_cap = 16;
    rows = (Row *)calloc(rows_cap, sizeof(Row));
}

/* ------------------------------------------------------------------- */
static void ensure_precompute(idx_t n)
{
    init_prime_list();
    init_rows();

    /* determine diagonal that contains a(n) */
    idx_t d = 1, sum = 1;
    while (sum < n) {
        ++d;
        sum += d;
    }

    if (d <= current_dmax) return;   /* already generated enough */

    /* extend rows array if necessary */
    if (rows_cap <= (size_t)d) {
        size_t new_cap = rows_cap ? rows_cap : 16;
        while (new_cap <= (size_t)d) new_cap <<= 1;
        rows = (Row *)realloc(rows, new_cap * sizeof(Row));
        for (size_t i = rows_cap; i < new_cap; ++i) {
            rows[i].len = 0;
            rows[i].cap = 0;
            rows[i].data = NULL;
        }
        rows_cap = new_cap;
    }

    /* generate integers until all required rows are filled */
    while (1) {
        bool all_filled = true;
        for (idx_t r = 1; r <= d; ++r) {
            idx_t need = d - r + 1;
            if (rows[r].len < need) {
                all_filled = false;
                break;
            }
        }
        if (all_filled) break;

        uint64_t m = next_num++;
        uint16_t r0 = row_of_number(m);

        /* ensure rows array can hold row r0 */
        if ((size_t)r0 + 1 > rows_cap) {
            size_t new_cap = rows_cap ? rows_cap : 16;
            while ((size_t)r0 + 1 > new_cap) new_cap <<= 1;
            rows = (Row *)realloc(rows, new_cap * sizeof(Row));
            for (size_t i = rows_cap; i < new_cap; ++i) {
                rows[i].len = 0;
                rows[i].cap = 0;
                rows[i].data = NULL;
            }
            rows_cap = new_cap;
        }

        Row *rowptr = &rows[r0];
        if (rowptr->len == rowptr->cap) {
            size_t new_cap = rowptr->cap ? rowptr->cap << 1 : 16;
            rowptr->data = (seq_t *)realloc(rowptr->data, new_cap * sizeof(seq_t));
            rowptr->cap = new_cap;
        }
        rowptr->data[rowptr->len++] = (seq_t)m;
    }
    current_dmax = d;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety, offset starts at 1 */
    ensure_precompute(n);

    /* find diagonal d that contains a(n) */
    idx_t d = 1, sum = 1;
    while (sum < n) {
        ++d;
        sum += d;
    }
    idx_t prev_sum = sum - d;
    idx_t pos_in_diag = n - prev_sum;          /* 1‑based position on diag */

    idx_t row = pos_in_diag;                    /* 1‑based row index  */
    idx_t col = d - row + 1;                    /* 1‑based column index */

    return rows[row].data[col - 1];
}
