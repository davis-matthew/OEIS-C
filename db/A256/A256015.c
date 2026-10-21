/*
 * A256015  Triangle read by rows: n-th row contains all distinct primes which
 *          are representable as the sum of some subset of the set of first n
 *          primes.
 * https://oeis.org/A256015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th term of the triangle where the r‑th row *
 *            consists of all primes that can be expressed as the sum of a
 *            subset * of the first r primes. The sequence is obtained by
 *            reading the * triangle row by row
 * Range:     u8 n <= 310, u16 n <= 1606, u32 n <= 1606, u64 n <= 1606,
 *            u128 n <= 1606, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n); */

/* ----------  static state used by the function  ---------- */

static idx_t   current_row = 0;      /* number of primes processed so far (0 at start) */
static idx_t   max_sum     = 0;      /* maximum sum reachable in current reach array */

static bool *  reach       = NULL;   /* bitset of reachable sums: 1 = reachable */
static size_t  reach_cap   = 0;      /* allocated capacity for reach (number of sums) */

/* storage of rows that have been generated */
static seq_t **  row_ptrs   = NULL;  /* array of pointers to prime arrays per row */
static size_t  *  row_len    = NULL;  /* length of each row */
static idx_t  *   cumul      = NULL;  /* cumulative term count up to each row */
static size_t    rows_cap   = 0;      /* capacity of row arrays */
static size_t    row_cnt    = 0;      /* number of rows stored */

/* ----------  helper functions  ---------- */

/* grow the reach array to hold at least new_cap elements (sums) */
static void grow_reach(idx_t new_cap)
{
    if (new_cap <= reach_cap) return;
    reach = (bool *)realloc(reach, new_cap * sizeof(bool));
    memset(reach + reach_cap, 0, (new_cap - reach_cap) * sizeof(bool));
    reach_cap = new_cap;
}

/* grow the row storage arrays to hold at least needed rows */
static void grow_rows(size_t needed)
{
    if (needed <= rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    row_ptrs = (seq_t **)realloc(row_ptrs, new_cap * sizeof(seq_t *));
    row_len  = (size_t *)realloc(row_len,  new_cap * sizeof(size_t));
    cumul    = (idx_t *)realloc(cumul,  new_cap * sizeof(idx_t));
    rows_cap = new_cap;
}

/* generate the next row (i.e., add the next prime to the subset sums) */
static void generate_next_row(void)
{
    ++current_row;                                 /* move to next prime index (1‑based) */
    seq_t prime = (seq_t)get_prime(current_row);   /* value of the prime */
    idx_t prime_idx = (idx_t)prime;                /* shift amount – fits in idx_t for our use */

    /* extend reach array to accommodate new sums */
    idx_t new_max = max_sum + prime_idx;
    grow_reach(new_max + 1);

    /* add the new prime to existing sums – iterate downwards to avoid reuse of
     * the same prime more than once in a single row */
    for (idx_t i = new_max; i >= 1; --i) {
        if (i >= prime_idx && reach[i - prime_idx]) reach[i] = true;
    }
    /* reach[0] remains true (empty subset) */
    max_sum = new_max;

    /* collect the primes among reachable sums */
    size_t primes_cap = 16;
    seq_t *primes_cur = (seq_t *)malloc(primes_cap * sizeof(seq_t));
    size_t primes_len = 0;

    for (idx_t s = 2; s <= max_sum; ++s) {
        if (reach[s] && is_prime_big((seq_t)s)) {
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes_cur = (seq_t *)realloc(primes_cur, primes_cap * sizeof(seq_t));
            }
            primes_cur[primes_len++] = (seq_t)s;
        }
    }

    /* store the row for future queries */
    grow_rows(row_cnt + 1);
    row_ptrs[row_cnt] = primes_cur;
    row_len[row_cnt]  = primes_len;
    cumul[row_cnt]    = (row_cnt == 0 ? (idx_t)primes_len
                                      : cumul[row_cnt - 1] + (idx_t)primes_len);
    ++row_cnt;
}

/* ----------  public function required by the harness  ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, return 0 as safe default */

    /* initialise reach if this is the first call */
    if (reach == NULL) {
        reach_cap = 1;
        reach = (bool *)malloc(reach_cap * sizeof(bool));
        reach[0] = true;          /* sum 0 is always reachable */
    }

    /* generate rows until the cumulative count reaches or exceeds n */
    while (row_cnt == 0 || cumul[row_cnt - 1] < n) {
        generate_next_row();
    }

    /* locate the row containing the n‑th term */
    size_t r = 0;
    while (r < row_cnt && cumul[r] < n) ++r;

    idx_t before = (r == 0) ? 0 : cumul[r - 1];
    size_t pos = (size_t)(n - before - 1);          /* 0‑based index within the row */
    return row_ptrs[r][pos];
}
