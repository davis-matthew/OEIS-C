/*
 * A371012  The largest prime that divides the n-th number that is the sum of
 *          2 squares; a(2) = 1.
 * https://oeis.org/A371012
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 8842, u32 n <= 8842, u64 n <= 8842,
 *            u128 n <= 8842, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A371012:  
   a(n) = the largest prime that divides the n-th number that is the sum of 2 squares  
   (offset 2: a(2) is the first term) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gpf.h"           /* uint64_t gpf(uint64_t) */

/* Test whether m is representable as a sum of two squares using the
   standard criterion on prime factors ≡3 (mod 4). */
static bool is_sum_of_two_squares(uint64_t m)
{
    if (m == 0 || m == 1) return true;
    uint64_t n = m;
    uint64_t p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if ((p & 3) == 3 && (exp & 1)) return false;
        }
        ++p;
    }
    if (n > 1) {                     /* remaining prime factor > 1 */
        if ((n & 3) == 3) return false;
    }
    return true;
}

/* Static storage for the sums of two squares and their largest prime
   factors.  The arrays grow dynamically as needed. */
static seq_t *sum_vals  = NULL;   /* A001481(n) up to current length      */
static seq_t *lpf_vals  = NULL;   /* A006530 of each stored sum           */
static size_t sum_cap   = 0;      /* current allocated capacity           */
static size_t sum_len   = 0;      /* number of values stored              */
static uint64_t next_n  = 1;      /* next integer to test (start at 1)    */

/* Ensure the dynamic arrays have at least 'need' capacity. */
static void grow_vectors(size_t need)
{
    if (sum_cap >= need) return;
    size_t new_cap = sum_cap ? sum_cap << 1 : 1024;
    while (new_cap < need) new_cap <<= 1;

    if (sum_vals == NULL) {
        sum_vals = (seq_t *)malloc(new_cap * sizeof(seq_t));
        lpf_vals = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        sum_vals = (seq_t *)realloc(sum_vals, new_cap * sizeof(seq_t));
        lpf_vals = (seq_t *)realloc(lpf_vals, new_cap * sizeof(seq_t));
    }
    sum_cap = new_cap;
}

/* Fill the tables up to at least 'target_index' (0‑based). */
static void ensure_upto(size_t target_index)
{
    if (sum_len > target_index) return;
    grow_vectors(target_index + 1);

    while (sum_len <= target_index) {
        if (is_sum_of_two_squares(next_n)) {
            seq_t val = (seq_t)next_n;
            seq_t lpf;
            if (next_n <= 1) {
                lpf = (seq_t)1;                /* convention: largest prime of 1 is 1 */
            } else {
                lpf = (seq_t)gpf(next_n);       /* gpf returns largest prime factor */
            }
            sum_vals[sum_len] = val;
            lpf_vals[sum_len] = lpf;
            ++sum_len;
        }
        ++next_n;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 2) {
        /* The sequence is defined for n >= 2 (offset 2). Return 1 for
           safety; this matches the first listed term. */
        return (seq_t)1;
    }
    size_t target = (size_t)(n - 2);   /* a(2) corresponds to sum_vals[0]   */
    ensure_upto(target);
    return lpf_vals[target];
}
