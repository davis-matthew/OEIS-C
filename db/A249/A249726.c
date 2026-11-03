/*
 * A249726  Numbers n such that there is a multiple of 36 on row n of Pascal's
 *          triangle with property that it is also the least multiple of 4 and
 *          the least multiple of 9 on the same row.
 * https://oeis.org/A249726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 4547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9871 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249726: compute by checking rows of Pascal's triangle for equality of
   the minimal distances to the nearest zero modulo 4 and modulo 9. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Count the number of borrows when subtracting k from n in base p.
   The number of borrows equals the exponent of p in C(n,k). */
static int count_p_borrows(idx_t n, idx_t k, int p)
{
    int v = 0;
    int borrow = 0;
    while (n || k || borrow) {
        int dn = n % p;
        int dk = k % p;
        int sum = dk + borrow;
        if (sum > dn) {
            ++v;
            borrow = 1;
        } else {
            borrow = 0;
        }
        n /= p;
        k /= p;
    }
    return v;
}

/* Find the smallest k (1 ≤ k ≤ ⌊n/2⌋) such that v_p(C(n,k)) ≥ 2.
   If a limit is given (>0), stop searching once k > limit.
   On success *out is set to k and true is returned, otherwise false. */
static bool find_min_prime_distance(idx_t n, int p, idx_t limit, idx_t *out)
{
    idx_t half = n >> 1;          /* floor(n/2) */
    idx_t upper = half;
    if (limit && limit < upper) upper = limit;

    for (idx_t k = 1; k <= upper; ++k) {
        if (count_p_borrows(n, k, p) >= 2) {
            *out = k;
            return true;
        }
    }
    return false;
}

/* Test whether row n satisfies the A249726 condition:
   the distance from the edge to the nearest zero is the same for
   modulo 4 and modulo 9. */
static bool is_good_row(idx_t n)
{
    idx_t dist4, dist9;
    if (!find_min_prime_distance(n, 2, 0, &dist4)) return false;
    if (!find_min_prime_distance(n, 3, dist4, &dist9)) return false;
    return dist4 == dist9;
}

/* External types are provided by the build. */
typedef uint64_t idx_t;

/* Cache for the sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static idx_t  seq_len   = 0;        /* number of terms stored */
static idx_t  next_row  = 1;        /* next row to test */

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    /* Extend cache until we have n terms. */
    while (seq_len < n) {
        idx_t row = next_row++;
        if (is_good_row(row)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
                if (!tmp) abort();                 /* out‑of‑memory */
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)row;
        }
    }
    return seq_cache[n - 1];
}
