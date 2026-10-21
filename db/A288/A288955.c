/*
 * A288955  Integers that are the sum of two consecutive semiprimes, as well
 *          as the product of two consecutive semiprimes.
 * https://oeis.org/A288955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th integer that is simultaneously the sum and the product
   of two consecutive semiprimes (possibly different pairs).
   The algorithm generates semiprimes in order, computes the sums
   and products of each consecutive pair, keeps the two sequences
   sorted, and collects their intersection.  The result array
   is built incrementally; indices 1,2,… are returned by a(n). */

/* Required includes */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* dynamic array helpers (realloc with cast for class types) */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed > *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/* test whether n is a semiprime: exactly two prime factors (counted
   with multiplicity). */
static bool is_semiprime(idx_t n)
{
    if (n < 4) return false;          /* smallest semiprime */
    idx_t cnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        while (temp % p == 0) {
            ++cnt;
            if (cnt > 2) return false;
            temp /= p;
        }
    }
    if (temp > 1) ++cnt;              /* remaining prime factor */
    return cnt == 2;
}

/* --------------------------------------------------------------------- */
/* global state for incremental construction */
static seq_t *sum_arr   = NULL;    /* sums of consecutive semiprimes */
static size_t sum_len   = 0, sum_cap   = 0;
static seq_t *prod_arr  = NULL;    /* products of consecutive semiprimes */
static size_t prod_len  = 0, prod_cap  = 0;
static seq_t *res_arr   = NULL;    /* intersection values in sorted order */
static size_t res_len   = 0, res_cap   = 0;
static idx_t  prev_semiprime = 0;   /* previous semiprime found */
static idx_t  next_check    = 1;     /* next integer to test for semiprime */
static size_t inter_si = 0, inter_pi = 0;    /* intersection pointers */

/* add new sum value and check against products */
static void process_sum(seq_t val)
{
    while (inter_pi < prod_len && prod_arr[inter_pi] < val) ++inter_pi;
    if (inter_pi < prod_len && prod_arr[inter_pi] == val) {
        if (res_len == 0 || res_arr[res_len-1] != val) {
            ensure_capacity(res_len + 1, &res_cap, &res_arr);
            res_arr[res_len++] = val;
        }
    }
}

/* add new product value and check against sums */
static void process_prod(seq_t val)
{
    while (inter_si < sum_len && sum_arr[inter_si] < val) ++inter_si;
    if (inter_si < sum_len && sum_arr[inter_si] == val) {
        if (res_len == 0 || res_arr[res_len-1] != val) {
            ensure_capacity(res_len + 1, &res_cap, &res_arr);
            res_arr[res_len++] = val;
        }
    }
}

/* extend the sequence until we have at least n (1‑based) terms */
static void extend_to(idx_t n)
{
    while (res_len < n) {      /* keep generating until enough terms */
        ++next_check;
        if (is_semiprime(next_check)) {
            if (prev_semiprime) {
                seq_t sum_val  = (seq_t)prev_semiprime + (seq_t)next_check;
                seq_t prod_val = (seq_t)prev_semiprime * (seq_t)next_check;

                /* store and process sums */
                ensure_capacity(sum_len + 1, &sum_cap, &sum_arr);
                sum_arr[sum_len++] = sum_val;
                process_sum(sum_val);

                /* store and process products */
                ensure_capacity(prod_len + 1, &prod_cap, &prod_arr);
                prod_arr[prod_len++] = prod_val;
                process_prod(prod_val);
            }
            prev_semiprime = next_check;
        }
    }
}

/* --------------------------------------------------------------------- */
/* public function requested by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for zero index, return 0 */
    extend_to(n);
    return res_arr[n-1];
}
