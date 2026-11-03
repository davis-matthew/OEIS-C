/*
 * A360061  Lexicographically earliest increasing sequence such that a(1) = 2
 *          and for n >= 2, a(1)^2 + a(2)^2 + ... + a(n)^2 is a prime.
 * https://oeis.org/A360061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest increasing sequence such that a(1)=2 and for
   n>=2 the sum of squares up to a(n) is prime.
   Algorithm:
   • Keep arrays of computed terms and cumulative sums of squares.
   • For each new index i, start at previous term +1 and increment
     until the new sum is prime (using is_prime_big).
   • Memoise computed terms so calls in any order are O(1) after
     initial construction.

   The function is thread‑local since all data structures are static.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Number of stored terms (including index 0 unused) */
static idx_t seq_len = 0;
/* Capacity of the allocated arrays */
static size_t seq_cap = 0;
/* Arrays holding sequence values and cumulative sums of squares */
static seq_t *seq_vals = NULL;
static seq_t *sum_squares = NULL;

/* Ensure the arrays can hold up to 'need' index (1‑based) */
static void ensure_size(idx_t need)
{
    if (need + 1 <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= need) new_cap *= 2;
    seq_vals = mem_resize(seq_t, seq_vals, new_cap);
    sum_squares = mem_resize(seq_t, sum_squares, new_cap);
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but harmless */
    ensure_size(n);

    /* initialise first term if needed */
    if (seq_len == 0) {
        seq_vals[1] = (seq_t)2;
        sum_squares[1] = seq_vals[1] * seq_vals[1]; /* 2^2 = 4 */
        seq_len = 1;
    }

    /* extend sequence up to requested n */
    while (seq_len < n) {
        idx_t i = seq_len + 1;
        seq_t prev = seq_vals[seq_len];
        seq_t cand = prev + 1u;           /* smallest candidate > previous */

        while (true) {
            seq_t cand_sq = cand * cand;         /* cand^2 */
            seq_t new_sum = sum_squares[seq_len] + cand_sq;
            if (is_prime_big(new_sum)) {
                seq_vals[i] = cand;
                sum_squares[i] = new_sum;
                break;
            }
            cand++;      /* try next larger integer */
        }
        seq_len = i;
    }
    return seq_vals[n];
}
